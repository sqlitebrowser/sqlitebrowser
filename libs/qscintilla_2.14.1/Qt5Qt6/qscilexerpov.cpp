// This module implements the QsciLexerPOV class.
//
// Copyright (c) 2023 Riverbank Computing Limited <info@riverbankcomputing.com>
// 
// This file is part of QScintilla.
// 
// This file may be used under the terms of the GNU General Public License
// version 3.0 as published by the Free Software Foundation and appearing in
// the file LICENSE included in the packaging of this file.  Please review the
// following information to ensure the GNU General Public License version 3.0
// requirements will be met: http://www.gnu.org/copyleft/gpl.html.
// 
// If you do not wish to use this file under the terms of the GPL version 3.0
// then you may purchase a commercial license.  For more information contact
// info@riverbankcomputing.com.
// 
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.


#include "Qsci/qscilexerpov.h"

#include <qcolor.h>
#include <qfont.h>
#include <qsettings.h>


// The ctor.
QsciLexerPOV::QsciLexerPOV(QObject *parent)
    : QsciLexer(parent),
      fold_comments(false), fold_compact(true), fold_directives(false) 
{
}


// The dtor.
QsciLexerPOV::~QsciLexerPOV()
{
}


// Returns the language name.
const char *QsciLexerPOV::language() const
{
    return "POV";
}


// Returns the lexer name.
const char *QsciLexerPOV::lexer() const
{
    return "pov";
}


// Return the style used for braces.
int QsciLexerPOV::braceStyle() const
{
    return Operator;
}


// Return the string of characters that comprise a word.
const char *QsciLexerPOV::wordCharacters() const
{
    return "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_#";
}


// Returns the foreground colour of the text for a style.
QColor QsciLexerPOV::defaultColor(int style) const
{
    switch (style)
    {
    case Default:
        return QColor(0xff,0x00,0x80);

    case Comment:
    case CommentLine:
        return QColor(0x00,0x7f,0x00);

    case Number:
        return QColor(0x00,0x7f,0x7f);

    case Operator:
        return QColor(0x00,0x00,0x00);

    case String:
        return QColor(0x7f,0x00,0x7f);

    case Directive:
        return QColor(0x7f,0x7f,0x00);

    case BadDirective:
        return QColor(0x80,0x40,0x20);

    case ObjectsCSGAppearance:
    case TypesModifiersItems:
    case PredefinedIdentifiers:
    case PredefinedFunctions:
    case KeywordSet6:
    case KeywordSet7:
    case KeywordSet8:
        return QColor(0x00,0x00,0x7f);
    }

    return QsciLexer::defaultColor(style);
}


// Returns the end-of-line fill for a style.
bool QsciLexerPOV::defaultEolFill(int style) const
{
    if (style == UnclosedString)
        return true;

    return QsciLexer::defaultEolFill(style);
}


// Returns the font of the text for a style.
QFont QsciLexerPOV::defaultFont(int style) const
{
    QFont f;

    switch (style)
    {
    case Comment:
    case CommentLine:
#if defined(Q_OS_WIN)
        f = QFont("Comic Sans MS",9);
#elif defined(Q_OS_MAC)
        f = QFont("Comic Sans MS", 12);
#else
        f = QFont("Bitstream Vera Serif",9);
#endif
        break;

    case UnclosedString:
    case PredefinedIdentifiers:
        f = QsciLexer::defaultFont(style);
        f.setBold(true);
        break;

    case BadDirective:
#if defined(Q_OS_WIN)
        f = QFont("Comic Sans MS",9);
#elif defined(Q_OS_MAC)
        f = QFont("Comic Sans MS", 12);
#else
        f = QFont("Bitstream Vera Serif",9);
#endif
        f.setItalic(true);
        break;

    default:
        f = QsciLexer::defaultFont(style);
    }

    return f;
}


// Returns the set of keywords.
const char *QsciLexerPOV::keywords(int set) const
{
    if (set == 1)
        return
            "declare local include undef fopen fclose read write "
            "default version case range break debug error "
            "warning if ifdef ifndef switch while macro else end";

    if (set == 2)
        return
            "camera light_source light_group object blob sphere "
            "cylinder box cone height_field julia_fractal lathe "
            "prism sphere_sweep superellipsoid sor text torus "
            "bicubic_patch disc mesh mesh2 polygon triangle "
            "smooth_triangle plane poly cubic quartic quadric "
            "isosurface parametric union intersection difference "
            "merge function array spline vertex_vectors "
            "normal_vectors uv_vectors face_indices "
            "normal_indices uv_indices texture texture_list "
            "interior_texture texture_map material_map image_map "
            "color_map colour_map pigment_map normal_map "
            "slope_map bump_map density_map pigment normal "
            "material interior finish reflection irid slope "
            "pigment_pattern image_pattern warp media scattering "
            "density background fog sky_sphere rainbow "
            "global_settings radiosity photons pattern transform "
            "looks_like projected_through contained_by "
            "clipped_by bounded_by";

    if (set == 3)
        return
            "linear_spline quadratic_spline cubic_spline "
            "natural_spline bezier_spline b_spline read write "
            "append inverse open perspective orthographic "
            "fisheye ultra_wide_angle omnimax panoramic "
            "spherical spotlight jitter circular orient "
            "media_attenuation media_interaction shadowless "
            "parallel refraction collect pass_through "
            "global_lights hierarchy sturm smooth gif tga iff "
            "pot png pgm ppm jpeg tiff sys ttf quaternion "
            "hypercomplex linear_sweep conic_sweep type "
            "all_intersections split_union cutaway_textures "
            "no_shadow no_image no_reflection double_illuminate "
            "hollow uv_mapping all use_index use_color "
            "use_colour no_bump_scale conserve_energy fresnel "
            "average agate boxed bozo bumps cells crackle "
            "cylindrical density_file dents facets granite "
            "leopard marble onion planar quilted radial ripples "
            "spotted waves wood wrinkles solid use_alpha "
            "interpolate magnet noise_generator toroidal "
            "ramp_wave triangle_wave sine_wave scallop_wave "
            "cubic_wave poly_wave once map_type method fog_type "
            "hf_gray_16 charset ascii utf8 rotate scale "
            "translate matrix location right up direction sky "
            "angle look_at aperture blur_samples focal_point "
            "confidence variance radius falloff tightness "
            "point_at area_light adaptive fade_distance "
            "fade_power threshold strength water_level tolerance "
            "max_iteration precision slice u_steps v_steps "
            "flatness inside_vector accuracy max_gradient "
            "evaluate max_trace precompute target ior dispersion "
            "dispersion_samples caustics color colour rgb rgbf "
            "rgbt rgbft red green blue filter transmit gray hf "
            "fade_color fade_colour quick_color quick_colour "
            "brick checker hexagon brick_size mortar bump_size "
            "ambient diffuse brilliance crand phong phong_size "
            "metallic specular roughness reflection_exponent "
            "exponent thickness gradient spiral1 spiral2 "
            "agate_turb form metric offset df3 coords size "
            "mandel exterior julia control0 control1 altitude "
            "turbulence octaves omega lambda repeat flip "
            "black-hole orientation dist_exp major_radius "
            "frequency phase intervals samples ratio absorption "
            "emission aa_threshold aa_level eccentricity "
            "extinction distance turb_depth fog_offset fog_alt "
            "width arc_angle falloff_angle adc_bailout "
            "ambient_light assumed_gamma irid_wavelength "
            "number_of_waves always_sample brigthness count "
            "error_bound gray_threshold load_file "
            "low_error_factor max_sample minimum_reuse "
            "nearest_count pretrace_end pretrace_start "
            "recursion_limit save_file spacing gather "
            "max_trace_level autostop expand_thresholds";

    if (set == 4)
        return
            "x y z t u v yes no true false on off clock "
            "clock_delta clock_on final_clock final_frame "
            "frame_number image_height image_width initial_clock "
            "initial_frame pi version";

    if (set == 5)
        return
            "abs acos acosh asc asin asinh atan atanh atan2 ceil "
            "cos cosh defined degrees dimensions dimension_size "
            "div exp file_exists floor inside int ln log max min "
            "mod pow prod radians rand seed select sin sinh sqrt "
            "strcmp strlen sum tan tanh val vdot vlength "
            "min_extent max_extent trace vaxis_rotate vcross "
            "vrotate vnormalize vturbulence chr concat str "
            "strlwr strupr substr vstr sqr cube reciprocal pwr";

    return 0;
}


// Returns the user name of a style.
QString QsciLexerPOV::description(int style) const
{
    switch (style)
    {
    case Default:
        return tr("Default");

    case Comment:
        return tr("Comment");

    case CommentLine:
        return tr("Comment line");

    case Number:
        return tr("Number");

    case Operator:
        return tr("Operator");

    case Identifier:
        return tr("Identifier");

    case String:
        return tr("String");

    case UnclosedString:
        return tr("Unclosed string");

    case Directive:
        return tr("Directive");

    case BadDirective:
        return tr("Bad directive");

    case ObjectsCSGAppearance:
        return tr("Objects, CSG and appearance");

    case TypesModifiersItems:
        return tr("Types, modifiers and items");

    case PredefinedIdentifiers:
        return tr("Predefined identifiers");

    case PredefinedFunctions:
        return tr("Predefined functions");

    case KeywordSet6:
        return tr("User defined 1");

    case KeywordSet7:
        return tr("User defined 2");

    case KeywordSet8:
        return tr("User defined 3");
    }

    return QString();
}


// Returns the background colour of the text for a style.
QColor QsciLexerPOV::defaultPaper(int style) const
{
    switch (style)
    {
    case UnclosedString:
        return QColor(0xe0,0xc0,0xe0);

    case ObjectsCSGAppearance:
        return QColor(0xff,0xd0,0xd0);

    case TypesModifiersItems:
        return QColor(0xff,0xff,0xd0);

    case PredefinedFunctions:
        return QColor(0xd0,0xd0,0xff);

    case KeywordSet6:
        return QColor(0xd0,0xff,0xd0);

    case KeywordSet7:
        return QColor(0xd0,0xd0,0xd0);

    case KeywordSet8:
        return QColor(0xe0,0xe0,0xe0);
    }

    return QsciLexer::defaultPaper(style);
}


// Refresh all properties.
void QsciLexerPOV::refreshProperties()
{
    setCommentProp();
    setCompactProp();
    setDirectiveProp();
}


// Read properties from the settings.
bool QsciLexerPOV::readProperties(QSettings &qs,const QString &prefix)
{
    int rc = true;

    fold_comments = qs.value(prefix + "foldcomments", false).toBool();
    fold_compact = qs.value(prefix + "foldcompact", true).toBool();
    fold_directives = qs.value(prefix + "folddirectives", false).toBool();

    return rc;
}


// Write properties to the settings.
bool QsciLexerPOV::writeProperties(QSettings &qs,const QString &prefix) const
{
    int rc = true;

    qs.setValue(prefix + "foldcomments", fold_comments);
    qs.setValue(prefix + "foldcompact", fold_compact);
    qs.setValue(prefix + "folddirectives", fold_directives);

    return rc;
}


// Return true if comments can be folded.
bool QsciLexerPOV::foldComments() const
{
    return fold_comments;
}


// Set if comments can be folded.
void QsciLexerPOV::setFoldComments(bool fold)
{
    fold_comments = fold;

    setCommentProp();
}


// Set the "fold.comment" property.
void QsciLexerPOV::setCommentProp()
{
    emit propertyChanged("fold.comment",(fold_comments ? "1" : "0"));
}


// Return true if folds are compact.
bool QsciLexerPOV::foldCompact() const
{
    return fold_compact;
}


// Set if folds are compact
void QsciLexerPOV::setFoldCompact(bool fold)
{
    fold_compact = fold;

    setCompactProp();
}


// Set the "fold.compact" property.
void QsciLexerPOV::setCompactProp()
{
    emit propertyChanged("fold.compact",(fold_compact ? "1" : "0"));
}


// Return true if directives can be folded.
bool QsciLexerPOV::foldDirectives() const
{
    return fold_directives;
}


// Set if directives can be folded.
void QsciLexerPOV::setFoldDirectives(bool fold)
{
    fold_directives = fold;

    setDirectiveProp();
}


// Set the "fold.directive" property.
void QsciLexerPOV::setDirectiveProp()
{
    emit propertyChanged("fold.directive",(fold_directives ? "1" : "0"));
}
