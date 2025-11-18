extends Node

class FormatterTest extends Formatter:
	func format_input(to_format : FormattedPlaceholder, options: Dictionary) -> String:
		return "Test";

class FormatterFactoryTest extends FormatterFactory:
	func create_formatter(locale : String) -> Formatter:
		return FormatterTest.new();

func _ready() -> void:
	var builder := MessageFormatterBuilder.new();
	
	var functions := FunctionRegistryBuilder.new();
	# TODO: This causes a memory crash:
	#functions.adopt_formatter("test", FormatterFactoryTest.new());
	#var registry : FunctionRegistry = functions.build();
	
	builder.pattern = "{$name :test}";
	#builder.pattern = tr("LOCALIZATION_TEST");
	builder.locale = TranslationServer.get_locale();
	#builder.function_registry = registry;
	var formatter := builder.build();
	var res = formatter.format_to_string({
		"name": "Username",
	});
	print(res);
