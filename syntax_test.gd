class_name SyntaxTest extends Node

## Documentation Comment
## This script is designed to test syntax highlighting font styles.
## Verify that italic and bold styles are applied correctly.

# Regular Comment
# This is a standard comment.

# Keywords (var, const, static, func)
const CONSTANT_VALUE: int = 42
static var static_var: float = 3.14
var member_variable: String = "String Literal"

# Annotations
@export var exported_var: int = 0
@onready var node_path: NodePath = ^"Node/Path"

# Function Definition
# 'func' should be bold (if configured)
# 'void' is a base type (bold?)
func _ready() -> void:
    # Control Flow Keywords (if, else, for, while, match, return)
    if true:
        print("Control Flow: if")
    else:
        print("Control Flow: else")

    for i in range(5):
        pass # 'pass' keyword

    # Base Types (Vector2, Color, etc.)
    var vec: Vector2 = Vector2(10, 20)
    var col: Color = Color.RED
    
    # Engine Types / Classes
    var sprite: Sprite2D = Sprite2D.new()
    var timer: Timer = Timer.new()

    # Numbers
    var integer_num = 123
    var float_num = 12.34
    var hex_num = 0xFF

    # Strings and StringNames
    var string_literal = "Hello World"
    var string_name = &"StringName"
    var multiline_string = """
    Multi-line
    String
    """

    # Keyword 'self'
    self.call_deferred("queue_free")

    # Member variable usage
    member_variable = "Updated"

    _test_function(vec)

func _test_function(param: Vector2) -> bool:
    # 'return' keyword
    return param.x > 0
