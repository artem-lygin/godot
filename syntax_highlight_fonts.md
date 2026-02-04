Allow SyntaxHighlighter to return font variations (Bold/Italic) in addition to Color

Current Status: Open

Applicable Version: 4.x

Describe the project you are working on

I am working on a Godot plugin designed to improve the scripting experience by extending the Editor's syntax highlighting. The goal is to allow users to semanticize their code not just with color, but with font variations—specifically making keywords (like func, var) bold or data types (like String, int) italic. This is a common feature in modern IDEs (VS Code, IntelliJ, etc.) that aids in readability and code scanning.

Describe the problem or limitation you are having in your project

Currently, the EditorSyntaxHighlighter and its base SyntaxHighlighter API allows plugins to define highlighting data via the _get_line_syntax_highlighting(line) method.

According to the documentation and source code, this method returns a Dictionary where keys are column indices and values are nested Dictionaries. However, the nested Dictionary only supports the "color" key.

If I attempt to pass other keys to control the font style, they are ignored by the TextEdit / CodeEdit rendering pipeline.

Current API behavior:

# syntax_highlighter.gd
func _get_line_syntax_highlighting(line):
    return {
        0: { "color": Color.RED } # Works
        5: { "color": Color.BLUE, "bold": true } # "bold" is ignored
    }


This limitation forces syntax highlighters to rely 100% on color, which can lead to "rainbow code" where too many colors are used to distinguish concepts that could be better distinguished by font weight or slant.

Describe the feature / enhancement and how it helps to overcome the problem or limitation

I propose extending the SyntaxHighlighter return dictionary to accept standard font style flags. Specifically, the rendering pipeline for CodeEdit (and TextEdit) should check for bold and italic boolean keys (or a font_style bitmask) in the syntax dictionary and apply the corresponding variation of the active font.

This would allow plugin creators and the core team to implement richer themes, such as:

Bold keywords (if, else, for).

Italic comments or docstrings.

Italic static types.

Bold class names in declarations.

Describe how your proposal will work, with code, pseudo-code, mock-ups, and/or diagrams

1. API Changes (SyntaxHighlighter)

The structure of the Dictionary returned by _get_line_syntax_highlighting would remain backward compatible, but simply acknowledge new keys.

Proposed Usage:

func _get_line_syntax_highlighting(line: int) -> Dictionary:
    var formatting = {}
    
    # Example: "func" is Pink and Bold
    formatting[0] = {
        "color": Color.PINK,
        "bold": true
    }
    
    # Example: "my_function" is Blue and Regular
    formatting[4] = {
        "color": Color.LIGHT_BLUE,
        "bold": false
    }
    
    return formatting


2. Engine Changes (C++)

The change would primarily need to happen in TextEdit (or CodeEdit)'s drawing logic. When iterating over the syntax highlighting cache to draw the text:

Look up the font associated with the TextEdit.

If bold is present and true, switch to the Bold variation of that font (or use TextServer to simulate bold if a specific variation isn't provided).

If italic is present and true, switch to the Italic variation.

Pseudo-code logic for the rendering loop:

// Inside TextEdit::_draw_line or similar
for (int i = 0; i < line.length(); i++) {
    Dictionary highlight_data = syntax_highlighter->get_data_at(line, i);
    
    Color draw_color = default_color;
    if (highlight_data.has("color")) {
        draw_color = highlight_data["color"];
    }
    
    Font current_font = theme_font;
    
    // --- PROPOSED ADDITION ---
    if (highlight_data.has("bold") && highlight_data["bold"]) {
        current_font = theme_font_bold; // Or utilize TextServer variation
    }
    if (highlight_data.has("italic") && highlight_data["italic"]) {
        current_font = theme_font_italic; 
    }
    // -------------------------

    draw_char(current_font, position, char, draw_color);
}


If this enhancement will not be used often, can it be worked around with a few lines of script?

No. The syntax highlighting drawing phase is handled internally by the C++ engine code for performance reasons. It is not exposed to GDScript.

While one could overlay a RichTextLabel on top of a CodeEdit, this causes massive desync issues, input blocking, and performance degradation, making it an invalid workaround for a code editor.

Is there a reason why this should be core and not an add-on in the asset library?

This must be core because it requires modifying the TextEdit / CodeEdit rendering pipeline to respect new keys in the syntax dictionary. An add-on cannot inject code into the engine's internal text drawing loop.

This feature is standard in almost all modern code editors (VS Code, Sublime, JetBrains), and adding it would allow Godot's script editor to reach parity with external editors in terms of visual customization.