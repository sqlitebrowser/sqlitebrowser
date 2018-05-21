#ifndef ROW_CACHE_H
#define ROW_CACHE_H

#include <vector>
#include <numeric>
#include <algorithm>
#include <stdexcept>

/**

   cache structure adapted to the existing access patterns in
   SqliteTableModel. handles many large segments with gaps in between
   well.

   logical structure resembling a std::vector<std::optional<T>>, but
   implementation avoids actually allocating space for the non-empty
   optionals, and supports (hopefully) more efficient insertion /
   deletion.

   actually, this is not even a "cache" - once set, elements are never
   thrown away to make space for new elements.

   TODO introduce maximum segment size?

**/
template <typename T>
class RowCache
{
public:
    typedef T value_type;

    /// constructs an empty cache
    explicit RowCache ();

    /// \returns number of cached rows
    size_t numSet () const;

    /// \returns number of segments
    size_t numSegments () const;

    /// \returns 1 if specified row is loaded, 0 otherwise
    size_t count (size_t pos) const;

    /// \returns specified row. \throws if not available
    const T & at (size_t pos) const;
          T & at (size_t pos);

    /// assigns value to specified row; may increase numSet() by one
    void set (size_t pos, T && value);

    /// insert new element; increases numSet() by one
    void insert (size_t pos, T && value);

    /// delete element; decreases numSet() by one
    void erase (size_t pos);

    /// reset to state after construction
    void clear ();

    /// given a range of rows (end is exclusive), narrow it in order
    /// to remove already-loaded rows from both ends.
    void smallestNonAvailableRange (size_t & row_begin, size_t & row_end) const;

private:
    /// a single segment containing contiguous entries
    struct Segment
    {
        size_t pos_begin;
        std::vector<T> entries;

        /// returns past-the-end position of this segment
        size_t pos_end () const { return pos_begin + entries.size(); }
    };

    /// collection of non-overlapping segments, in order of increasing
    /// position
    using Segments = std::vector<Segment>;
    Segments segments;

    // ------------------------------------------------------------------------------

    /// \returns first segment that definitely cannot contain 'pos',
    /// because it starts at some later position.
    typename Segments::const_iterator getSegmentBeyond (size_t pos) const {
        // first segment whose pos_begin > pos (so can't contain pos itself):
        return std::upper_bound(segments.begin(), segments.end(), pos, pred);
    }

    typename Segments::iterator getSegmentBeyond (size_t pos) {
        return std::upper_bound(segments.begin(), segments.end(), pos, pred);
    }

    static bool pred (size_t pos, const Segment & s) { return pos < s.pos_begin; }

    // ------------------------------------------------------------------------------

    /// \returns segment containing 'pos'
    typename Segments::const_iterator getSegmentContaining (size_t pos) const
    {
        auto it = getSegmentBeyond(pos);

        if(it != segments.begin()) {
            auto prev_it = it - 1;
            if(pos < prev_it->pos_end())
                return prev_it;
        }

        return segments.end();
    }

};

template <typename T>
RowCache<T>::RowCache ()
{
}

template <typename T>
size_t RowCache<T>::numSet () const
{
    return std::accumulate(segments.begin(), segments.end(), size_t(0),
                           [](size_t r, const Segment & s) { return r + s.entries.size(); });
}

template <typename T>
size_t RowCache<T>::numSegments () const
{
    return segments.size();
}

template <typename T>
size_t RowCache<T>::count (size_t pos) const
{
    return getSegmentContaining(pos) != segments.end();
}

template <typename T>
const T & RowCache<T>::at (size_t pos) const
{
    auto it = getSegmentContaining(pos);

    if(it != segments.end())
        return it->entries[pos - it->pos_begin];

    throw std::out_of_range("no matching segment found");
}

template <typename T>
T & RowCache<T>::at (size_t pos)
{
    return const_cast<T&>(static_cast<const RowCache &>(*this).at(pos));
}

template <typename T>
void RowCache<T>::set (size_t pos, T && value)
{
    auto it = getSegmentBeyond(pos);

    if(it != segments.begin())
    {
        auto prev_it = it - 1;
        auto d = pos - prev_it->pos_begin; // distance from segment start (>=0)

        if(d < prev_it->entries.size())
        {
            // replace value
            prev_it->entries[d] = std::move(value);
            return;
        }

        if(d == prev_it->entries.size())
        {
            // extend existing segment
            prev_it->entries.insert(prev_it->entries.end(), std::move(value));
            return;
        }
    }

    // make new segment
    segments.insert(it, { pos, { std::move(value) } });
}

template <typename T>
void RowCache<T>::insert (size_t pos, T && value)
{
    auto it = getSegmentBeyond(pos);

    if(it != segments.begin())
    {
        auto prev_it = it - 1;
        auto d = pos - prev_it->pos_begin; // distance from segment start (>=0)

        if(d <= prev_it->entries.size())
        {
            // can extend existing segment
            prev_it->entries.insert(prev_it->entries.begin() + d, std::move(value));
            goto push;
        }
    }

    // make new segment
    it = segments.insert(it, { pos, { std::move(value) } }) + 1;

push:
    // push back all later segments
    std::for_each(it, segments.end(), [](Segment &s){ s.pos_begin++; });
}

template <typename T>
void RowCache<T>::erase (size_t pos)
{
    auto it = getSegmentBeyond(pos);

    // if previous segment actually contains pos, shorten it
    if(it != segments.begin())
    {
        auto prev_it = it - 1;
        auto d = pos - prev_it->pos_begin; // distance from segment start (>=0)

        if(d < prev_it->entries.size())
        {
            prev_it->entries.erase(prev_it->entries.begin() + d);
            if(prev_it->entries.empty())
            {
                it = segments.erase(prev_it);
            }
        }
    }

    // pull forward all later segments
    std::for_each(it, segments.end(), [](Segment &s){ s.pos_begin--; });
}

template <typename T>
void RowCache<T>::clear ()
{
    segments.clear();
}

template <typename T>
void RowCache<T>::smallestNonAvailableRange (size_t & row_begin, size_t & row_end) const
{
    if(row_end < row_begin)
        throw std::invalid_argument("end must be >= begin");

    while(row_begin < row_end) {
        auto it = getSegmentContaining(row_begin);
        if(it == segments.end())
            break;
        row_begin = it->pos_end();
    }

    while(row_end > row_begin) {
        auto it = getSegmentContaining(row_end - 1);
        if(it == segments.end())
            break;
        row_end = it->pos_begin;
    }

    if(row_end < row_begin)
        row_end = row_begin;
}

#endif // SEGMENTING_CACHE_H
