/**************************************************************************/
/*  gdscript_highlighter.h                                                */
/**************************************************************************/
/*                         This file is part of:                          */
/*                             GODOT ENGINE                               */
/*                        https://godotengine.org                         */
/**************************************************************************/
/* Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md). */
/* Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.                  */
/*                                                                        */
/* Permission is hereby granted, free of charge, to any person obtaining  */
/* a copy of this software and associated documentation files (the        */
/* "Software"), to deal in the Software without restriction, including    */
/* without limitation the rights to use, copy, modify, merge, publish,    */
/* distribute, sublicense, and/or sell copies of the Software, and to     */
/* permit persons to whom the Software is furnished to do so, subject to  */
/* the following conditions:                                              */
/*                                                                        */
/* The above copyright notice and this permission notice shall be         */
/* included in all copies or substantial portions of the Software.        */
/*                                                                        */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,        */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF     */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. */
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY   */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,   */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE      */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                 */
/**************************************************************************/

#pragma once

#include "editor/script/syntax_highlighters.h"

#include "core/math/color.h"
#include "core/string/string_name.h"
#include "core/templates/hash_map.h"
#include "core/templates/vector.h"

class GDScriptSyntaxHighlighter : public EditorSyntaxHighlighter {
	GDCLASS(GDScriptSyntaxHighlighter, EditorSyntaxHighlighter)

private:
	struct ColorRegion {
		enum Type {
			TYPE_NONE,
			TYPE_STRING, // `"` and `'`, optional prefix `&`, `^`, or `r`.
			TYPE_MULTILINE_STRING, // `"""` and `'''`, optional prefix `r`.
			TYPE_COMMENT, // `#` and `##`.
			TYPE_CODE_REGION, // `#region` and `#endregion`.
		};

		Type type = TYPE_NONE;
		Color color;
		int font_style = 0;
		String start_key;
		String end_key;
		bool line_only = false;
		bool r_prefix = false;
		bool is_string = false; // `TYPE_STRING` or `TYPE_MULTILINE_STRING`.
		bool is_comment = false; // `TYPE_COMMENT` or `TYPE_CODE_REGION`.
	};
	Vector<ColorRegion> color_regions;
	HashMap<int, int> color_region_cache;

	HashMap<StringName, Color> class_names;
	HashMap<StringName, int> class_names_style;
	HashMap<StringName, Color> reserved_keywords;
	HashMap<StringName, int> reserved_keywords_style;
	HashMap<StringName, Color> member_keywords;
	HashMap<StringName, int> member_keywords_style;
	HashSet<StringName> global_functions;

	enum Type {
		NONE,
		REGION,
		NODE_PATH,
		NODE_REF,
		ANNOTATION,
		STRING_NAME,
		SYMBOL,
		NUMBER,
		FUNCTION,
		SIGNAL,
		KEYWORD,
		MEMBER,
		IDENTIFIER,
		TYPE,
	};

	// Colors.
	Color font_color;
	Color symbol_color;
	Color function_color;
	Color global_function_color;
	Color function_definition_color;
	Color built_in_type_color;
	Color number_color;
	Color member_variable_color;
	Color string_color;
	Color placeholder_color;
	Color node_path_color;
	Color node_ref_color;
	Color annotation_color;
	Color string_name_color;
	Color type_color;

	int font_style = 0;
	int symbol_font_style = 0;
	int function_font_style = 0;
	int global_function_font_style = 0;
	int function_definition_font_style = 0;
	int built_in_type_font_style = 0;
	int number_font_style = 0;
	int member_variable_font_style = 0;
	int string_font_style = 0;
	int node_path_font_style = 0;
	int node_ref_font_style = 0;
	int annotation_font_style = 0;
	int string_name_font_style = 0;
	int type_font_style = 0;
	int text_font_style = 0;

	enum CommentMarkerLevel {
		COMMENT_MARKER_CRITICAL,
		COMMENT_MARKER_WARNING,
		COMMENT_MARKER_NOTICE,
		COMMENT_MARKER_MAX,
	};
	Color comment_marker_colors[COMMENT_MARKER_MAX];
	HashMap<String, CommentMarkerLevel> comment_markers;

	int comment_font_style = 0;
	int doc_comment_font_style = 0;

	void add_color_region(ColorRegion::Type p_type, const String &p_start_key, const String &p_end_key, const Color &p_color, bool p_line_only = false, bool p_r_prefix = false, int p_font_style = 0);

public:
	virtual void _update_cache() override;
	virtual Dictionary _get_line_syntax_highlighting_impl(int p_line) override;

	virtual String _get_name() const override;
	virtual PackedStringArray _get_supported_languages() const override;

	virtual Ref<EditorSyntaxHighlighter> _create() const override;
};
