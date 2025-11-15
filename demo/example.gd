extends Node


func _ready() -> void:
	var builder := MessageFormatterBuilder.new();
	
	builder.pattern = tr("LOCALIZATION_TEST");
	var formatter := builder.build();
	var res = formatter.format_to_string({
		"name": "Username",
	});
	print(res);
