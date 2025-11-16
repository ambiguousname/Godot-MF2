extends Node


func _ready() -> void:
	var builder := MessageFormatterBuilder.new();
	
	var functions := FunctionRegistryBuilder.new();
	var registry : FunctionRegistry = functions.build();
	
	builder.pattern = tr("LOCALIZATION_TEST");
	builder.locale = TranslationServer.get_locale();
	var formatter := builder.build();
	var res = formatter.format_to_string({
		"name": "Username",
	});
	print(res);
