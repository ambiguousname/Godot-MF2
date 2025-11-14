extends Node


func _ready() -> void:
	var formatter := MessageFormatterBuilder.new();
	var test : String = "Do what John {$name}?";
	formatter.set_pattern(test.to_utf8_buffer());
	var f : MessageFormatter = formatter.build();
	print(f);
	#var res = f.format_to_string({
		#"name": 2
	#});
	#print(res);
