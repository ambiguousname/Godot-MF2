extends Node


func _ready() -> void:
	var formatter := MessageFormatterBuilder.new();
	var test : String = "Do what John?";
	formatter.set_pattern(test.to_utf8_buffer());
	var f : MessageFormatter = formatter.build();
