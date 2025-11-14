extends Node


func _ready() -> void:
	var builder := MessageFormatterBuilder.new();
	var test : String = "Do what John {$name}?";
	builder.pattern = test.to_utf8_buffer();
	var formatter := builder.build();
	var res = formatter.format_to_string({
		"name": 3
	});
	print(res.get_string_from_utf8());
