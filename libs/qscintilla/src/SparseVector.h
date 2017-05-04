// Scintilla source code edit control
/** @file SparseVector.h
 ** Hold data sparsely associated with elements in a range.
 **/
// Copyright 2016 by Neil Hodgson <neilh@scintilla.org>
// The License.txt file describes the conditions under which this software may be distributed.

#ifndef SPARSEVECTOR_H
#define SPARSEVECTOR_H

#ifdef SCI_NAMESPACE
namespace Scintilla {
#endif

// SparseVector is similar to RunStyles but is more efficient for cases where values occur
// for one position instead of over a range of positions.
template <typename T>
class SparseVector {
private:
	Partitioning *starts;
	SplitVector<T> *values;
	// Private so SparseVector objects can not be copied
	SparseVector(const SparseVector &);
	void ClearValue(int partition) {
		values->SetValueAt(partition, T());
	}
	void CommonSetValueAt(int position, T value) {
		// Do the work of setting the value to allow for specialization of SetValueAt.
		assert(position < Length());
		const int partition = starts->PartitionFromPosition(position);
		const int startPartition = starts->PositionFromPartition(partition);
		if (value == T()) {
			// Setting the empty value is equivalent to deleting the position
			if (position == 0) {
				ClearValue(partition);
			} else if (position == startPartition) {
				// Currently an element at this position, so remove
				ClearValue(partition);
				starts->RemovePartition(partition);
				values->Delete(partition);
			}
			// Else element remains empty
		} else {
			if (position == startPartition) {
				// Already a value at this position, so replace
				ClearValue(partition);
				values->SetValueAt(partition, value);
			} else {
				// Insert a new element
				starts->InsertPartition(partition + 1, position);
				values->InsertValue(partition + 1, 1, value);
			}
		}
	}
public:
	SparseVector() {
		starts = new Partitioning(8);
		values = new SplitVector<T>();
		values->InsertValue(0, 2, T());
	}
	~SparseVector() {
		delete starts;
		starts = NULL;
		// starts dead here but not used by ClearValue.
		for (int part = 0; part < values->Length(); part++) {
			ClearValue(part);
		}
		delete values;
		values = NULL;
	}
	int Length() const {
		return starts->PositionFromPartition(starts->Partitions());
	}
	int Elements() const {
		return starts->Partitions();
	}
	int PositionOfElement(int element) const {
		return starts->PositionFromPartition(element);
	}
	T ValueAt(int position) const {
		assert(position < Length());
		const int partition = starts->PartitionFromPosition(position);
		const int startPartition = starts->PositionFromPartition(partition);
		if (startPartition == position) {
			return values->ValueAt(partition);
		} else {
			return T();
		}
	}
	void SetValueAt(int position, T value) {
		CommonSetValueAt(position, value);
	}
	void InsertSpace(int position, int insertLength) {
		assert(position <= Length());	// Only operation that works at end.
		const int partition = starts->PartitionFromPosition(position);
		const int startPartition = starts->PositionFromPartition(partition);
		if (startPartition == position) {
			T valueCurrent = values->ValueAt(partition);
			// Inserting at start of run so make previous longer
			if (partition == 0) {
				// Inserting at start of document so ensure 0
				if (valueCurrent != T()) {
					ClearValue(0);
					starts->InsertPartition(1, 0);
					values->InsertValue(1, 1, valueCurrent);
					starts->InsertText(0, insertLength);
				} else {
					starts->InsertText(partition, insertLength);
				}
			} else {
				if (valueCurrent != T()) {
					starts->InsertText(partition - 1, insertLength);
				} else {
					// Insert at end of run so do not extend style
					starts->InsertText(partition, insertLength);
				}
			}
		} else {
			starts->InsertText(partition, insertLength);
		}
	}
	void DeletePosition(int position) {
		assert(position < Length());
		int partition = starts->PartitionFromPosition(position);
		const int startPartition = starts->PositionFromPartition(partition);
		if (startPartition == position) {
			if (partition == 0) {
				ClearValue(0);
			} else if (partition == starts->Partitions()) {
				// This should not be possible
				ClearValue(partition);
				throw std::runtime_error("SparseVector: deleting end partition.");
			} else {
				ClearValue(partition);
				starts->RemovePartition(partition);
				values->Delete(partition);
				// Its the previous partition now that gets smaller 
				partition--;
			}
		}
		starts->InsertText(partition, -1);
	}
	void Check() const {
		if (Length() < 0) {
			throw std::runtime_error("SparseVector: Length can not be negative.");
		}
		if (starts->Partitions() < 1) {
			throw std::runtime_error("SparseVector: Must always have 1 or more partitions.");
		}
		if (starts->Partitions() != values->Length() - 1) {
			throw std::runtime_error("SparseVector: Partitions and values different lengths.");
		}
		// The final element can not be set
		if (values->ValueAt(values->Length() - 1) != T()) {
			throw std::runtime_error("SparseVector: Unused style at end changed.");
		}
	}
};

// The specialization for const char * makes copies and deletes them as needed.

template<>
inline void SparseVector<const char *>::ClearValue(int partition) {
	const char *value = values->ValueAt(partition);
	delete []value;
	values->SetValueAt(partition, NULL);
}

template<>
inline void SparseVector<const char *>::SetValueAt(int position, const char *value) {
	// Make a copy of the string
	if (value) {
		const size_t len = strlen(value);
		char *valueCopy = new char[len + 1]();
		std::copy(value, value + len, valueCopy);
		CommonSetValueAt(position, valueCopy);
	} else {
		CommonSetValueAt(position, NULL);
	}
}

#ifdef SCI_NAMESPACE
}
#endif

#endif
