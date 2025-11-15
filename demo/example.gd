extends Node


func _ready() -> void:
	var builder := MessageFormatterBuilder.new();
	
	var string_to_localize = tr("LOCALIZATION_TEST");
	builder.pattern = string_to_localize.to_utf8_buffer();
	var formatter := builder.build();
	var res = formatter.format_to_string({
		"name": "Username",
	});
	print(res);
