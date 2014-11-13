
#include <configManager.h>
#include <amxxmodule.h>

#include <global.h>
#include <filesManager.h>

#include <ctime>

#include <function.h>
#include <hooker.h>

#include <functionStructure.h>

#include <memoryStructure.h>

const char *OperativeSystems[] ={ "linux", "windows" };

#if defined __linux__
const char *OperativeSystem = OperativeSystems[0];
#else
const char *OperativeSystem = OperativeSystems[1];
#endif

ConfigManager::ConfigManager()
{
	obtainPaths();
}

void ConfigManager::obtainPaths()
{
	char modDir[100];
	char path[256];

	g_engfuncs.pfnGetGameDir(modDir);
	char* configsDir = LOCALINFO("amxx_configsdir");

	Global::Modname = modDir;

	UTIL_Format(path, sizeof(path) - 1, "%s/%s/%s", modDir, configsDir, Global::OrpheuFolders.config.chars());
	orpheuPaths.config = path;

	UTIL_Format(path, sizeof(path) - 1, "%s/%s/", orpheuPaths.config.chars(), Global::OrpheuFolders.functions.chars());
	orpheuPaths.functions = path;

	UTIL_Format(path, sizeof(path) - 1, "%s/%s/", orpheuPaths.config.chars(), Global::OrpheuFolders.libraries.chars());
	orpheuPaths.libraries = path;

	UTIL_Format(path, sizeof(path) - 1, "%s/%s/", orpheuPaths.config.chars(), Global::OrpheuFolders.typeAliases.chars());
	orpheuPaths.typeAliases = path;

	UTIL_Format(path, sizeof(path) - 1, "%s/%s/", orpheuPaths.config.chars(), Global::OrpheuFolders.virtualFunctions.chars());
	orpheuPaths.virtualFunctions = path;

	UTIL_Format(path, sizeof(path) - 1, "%s/%s/", orpheuPaths.config.chars(), Global::OrpheuFolders.mods.chars());
	orpheuPaths.mods = path;

	UTIL_Format(path, sizeof(path) - 1, "%s/%s/", orpheuPaths.config.chars(), Global::OrpheuFolders.memory.chars());
	orpheuPaths.memory = path;
}

void ConfigManager::loadFunctions()
{
	Global::ConfigManagerObj->ModuleConfig.append(ke::AString("\nOrpheu functions search started.\n"));

	Global::ConfigManagerObj->ModuleConfig.append(ke::AString("\n\tParsing functions started.\n\n"));

	parseFunctionsInfo();

	Global::ConfigManagerObj->ModuleConfig.append(ke::AString("\tParsing functions ended.\n\n"));

	Global::ConfigManagerObj->ModuleConfig.append(ke::AString("\nOrpheu functions search ended.\n"));
}

void ConfigManager::loadVirtualFunctions()
{
	Global::ConfigManagerObj->ModuleConfig.append(ke::AString("\nOrpheu virtual functions search started.\n"));

	Global::ConfigManagerObj->ModuleConfig.append(ke::AString("\n\tParsing virtual functions started.\n\n"));

	parseVirtualFunctionsInfo();

	Global::ConfigManagerObj->ModuleConfig.append(ke::AString("\n\tParsing virtual functions ended.\n"));

	Global::ConfigManagerObj->ModuleConfig.append(ke::AString("\nOrpheu virtual functions search ended.\n"));
}

void ConfigManager::parseModsInfo()
{
	char msg[100];
	char path[256];

	ke::Vector<ke::AString>* files = FilesManager::getFiles(orpheuPaths.mods.chars());

	UTIL_Format(msg, sizeof(msg) - 1, "\t\tCurrent mod : \"%s\"\n\n", Global::Modname.chars());
	Global::ConfigManagerObj->ModuleConfig.append(ke::AString(msg));

	for (unsigned i=0; i < files->length(); ++i)
	{
		UTIL_Format(path, sizeof(path) - 1, "%s/%s", orpheuPaths.mods.chars(), files->at(i).chars());

		UTIL_Format(msg, sizeof(msg) - 1, "\t\tParsing mod file \"%s\"\n", files->at(i).chars());
		Global::ConfigManagerObj->ModuleConfig.append(ke::AString(msg));

		json_error_t error;
		json_t *root = json_load_file(path, 0, &error);

		bool foundMod = false;
		bool parsingSuccessful = !json_is_null(root);

		if (parsingSuccessful)
		{
			json_t *name = json_object_get(root, "name");

			if (!json_is_string(name))
			{
				UTIL_Format(msg, sizeof(msg) - 1, "\t\tFile must have a \"name\" field");
				Global::ConfigManagerObj->ModuleConfig.append(ke::AString(msg));
			}
			else
			{
				if (Global::Modname.compare(json_string_value(name)) != 0)
				{
					json_t *aliases = json_object_get(root, "aliases");

					if (aliases)
					{
						if (json_is_array(aliases))
						{
							for (size_t j = 0; j < json_array_size(aliases); ++j)
							{
								json_t *alias = json_array_get(aliases, j);

								if (!json_is_string(alias))
								{
									UTIL_Format(msg, sizeof(msg) - 1, "\t\t\tAll aliases must be strings\n");
									Global::ConfigManagerObj->ModuleConfig.append(ke::AString(msg));
									break;
								}

								if (Global::Modname.compare(json_string_value(alias)) == 0)
								{
									UTIL_Format(msg, sizeof(msg) - 1, "\t\t\tFound an alias.\n");
									Global::ConfigManagerObj->ModuleConfig.append(ke::AString(msg));
									Global::Modname = json_string_value(name);
									foundMod = true;
									break;
								}
							}
						}
						else
						{
							UTIL_Format(msg, sizeof(msg) - 1, "\t\t\tAliases must be an array\n");
							Global::ConfigManagerObj->ModuleConfig.append(ke::AString(msg));
						}
					}
				}
				else
				{
					UTIL_Format(msg, sizeof(msg) - 1, "\t\t\tFound.\n");
					Global::ConfigManagerObj->ModuleConfig.append(ke::AString(msg));

					foundMod = true;
					Global::Modname = json_string_value(name);
				}

				if (foundMod)
				{
					json_t *pev = json_object_get(root, "pev");

					if (json_is_object(pev))
					{
						json_t *value = json_object_get(pev, OperativeSystem);

						if (json_is_number(value))
						{
							Global::pev = (int)json_number_value(value);
						}
					}

					break;
				}
			}
		}
		else
		{
			UTIL_Format(msg, sizeof(msg) - 1, "\t\t\tFile incorrectly formatted\"%s\"\n", files->at(i).chars());
			Global::ConfigManagerObj->ModuleConfig.append(ke::AString(msg));
		}
	}
}

void ConfigManager::loadBaseData()
{
	Global::ConfigManagerObj->ModuleConfig.append(ke::AString("\nOrpheu configuration started.\n"));

	Global::ConfigManagerObj->ModuleConfig.append(ke::AString("\n\tParsing mods configuration started.\n\n"));

	parseModsInfo();

	Global::ConfigManagerObj->ModuleConfig.append(ke::AString("\n\tParsing mods configuration ended.\n"));

	Global::ConfigManagerObj->ModuleConfig.append(ke::AString("\n\tParsing libraries configuration started.\n\n"));

	Global::LibrariesCvarToName = parseExternalLibrariesInfo();

	Global::ConfigManagerObj->ModuleConfig.append(ke::AString("\n\tParsing libraries configuration ended.\n"));

	Global::ConfigManagerObj->ModuleConfig.append(ke::AString("\n\tParsing type aliases started.\n\n"));

	StringHashMap<long> typeNameToVirtualTableOffset;
	StringHashMap<ke::Vector<char*>*>* typeAliasesInfo = parseTypeAliasesInfo(typeNameToVirtualTableOffset);

	Global::ConfigManagerObj->ModuleConfig.append(ke::AString("\n\tParsing type aliases ended.\n"));

	Global::TypeHandlerManagerObj = new TypeHandlerManager(typeAliasesInfo);

	for (size_t i = 0; i < Global::TypesCount; ++i)
	{
		Global::TypeHandlerManagerObj->registerTypeHandler(Global::Types[i]->getName(), Global::Types[i]);

		long virtualTableOffset;

		if (typeNameToVirtualTableOffset.retrieve(Global::Types[i]->getName(), &virtualTableOffset))
		{
			Global::Types[i]->setVirtualTableOffset(virtualTableOffset);
		}
	}

	for (size_t i = 0; i < Global::StructuresCount; ++i)
		Global::TypeHandlerManagerObj->registerTypeHandler(Global::Structures[i]->getName(), Global::Structures[i]);

	Global::ConfigManagerObj->ModuleConfig.append(ke::AString("\nOrpheu configuration ended.\n"));
}

bool validateFile(ke::AString folder, ke::AString filename, ke::AString classname, json_t **root)
{
	char msg[100];
	char path[256];

	UTIL_Format(path, sizeof(path) - 1, "%s%s", folder.chars(), filename.chars());

	json_error_t error;
	*root = json_load_file(path, 0, &error);

	bool parsingSuccessful = !json_is_null(*root);

	if (parsingSuccessful)
	{
		json_t *key_name  = json_object_get(*root, "name");
		json_t *key_class = json_object_get(*root, "class");

		if (json_is_string(key_name))
		{
			if (filename.compare(json_string_value(key_name)) == 0)
			{
				if (classname.length())
				{
					if (json_is_string(key_class) && classname.compare(json_string_value(key_class)) == 0)
					{
						char classtype[32];
						UTIL_Format(classtype, sizeof(classtype) - 1, "%s *", classname.chars());

						if (!Global::TypeHandlerManagerObj->typeHandlerExists(classtype))
						{
							Global::ConfigManagerObj->ModuleConfig.append(ke::AString("\t\t\t\tClass type not registered\n"));
							return false;
						}
					}
					else
					{
						Global::ConfigManagerObj->ModuleConfig.append(ke::AString("\t\t\t\tClass name must be equal to the folder name\n"));
						return false;
					}
				}

				json_t *library = json_object_get(*root, "library");

				if (json_is_string(library))
				{
					if (LibrariesManager::hasLibrary(json_string_value(library)))
					{
						json_t *arguments = json_object_get(*root, "arguments");

						if (arguments)
						{
							if (json_is_array(arguments))
							{
								for (size_t i = 0; i < json_array_size(arguments); ++i)
								{
									json_t *argument = json_array_get(arguments, i);
									json_t *type = json_object_get(argument, "type");

									if (json_is_string(type))
									{
										if (!Global::TypeHandlerManagerObj->typeHandlerExists((char*)json_string_value(type)))
										{
											UTIL_Format(msg, sizeof(msg) - 1, "\t\t\t\tArgument has invalid type \"%s\"\n", json_string_value(type));
											Global::ConfigManagerObj->ModuleConfig.append(ke::AString(msg));
											return false;
										}
#ifndef _ERRORS_ONLY_
										else
										{
											UTIL_Format(msg, sizeof(msg) - 1, "\t\t\t\tArgument type \"%s\" validated\n", json_string_value(type));
											Global::ConfigManagerObj->ModuleConfig.append(ke::AString(msg));
										}
#endif
									}
									else
									{
										Global::ConfigManagerObj->ModuleConfig.append(ke::AString("\t\t\t\tArguments must have a type and it must be a string\n"));
										return false;
									}
								}
							}
							else
							{
								Global::ConfigManagerObj->ModuleConfig.append(ke::AString("\t\t\t\tArguments must be an array\n"));
								return false;
							}
						}

						json_t *return_ = json_object_get(*root, "return");

						if (return_)
						{
							json_t *type = json_object_get(return_, "type");

							if (json_is_string(type))
							{
								if (!Global::TypeHandlerManagerObj->typeHandlerExists((char*)json_string_value(type)))
								{
									UTIL_Format(msg, sizeof(msg) - 1, "\t\t\t\tReturn has invalid type \"%s\"\n", json_string_value(type));
									Global::ConfigManagerObj->ModuleConfig.append(ke::AString(msg));
									return false;
								}
#ifndef _ERRORS_ONLY_
								else
								{
									UTIL_Format(msg, sizeof(msg) - 1, "\t\t\t\tReturn type \"%s\" validated\n", json_string_value(type));
									Global::ConfigManagerObj->ModuleConfig.append(ke::AString(msg));
								}
#endif
							}
							else
							{
								Global::ConfigManagerObj->ModuleConfig.append(ke::AString("\t\t\t\tReturn must have a type and it must be a string\n"));
								return false;
							}
						}

						json_t *identifiers = json_object_get(*root, "identifiers");

						if (json_is_array(identifiers))
						{
							for (size_t i = 0; i < json_array_size(identifiers); ++i)
							{
								json_t *identifier = json_array_get(identifiers, i);
								json_t *os = json_object_get(identifier, "os");

								if (!json_is_string(os) || (strcmp(json_string_value(os), OperativeSystems[0]) != 0 && strcmp(json_string_value(os), OperativeSystems[1]) != 0))
								{
									UTIL_Format(msg, sizeof(msg) - 1, "\t\t\t\tIdentifier empty or containing an invalid operative system\n");
									Global::ConfigManagerObj->ModuleConfig.append(ke::AString("\t\t\t\tIdentifier empty or containing an invalid operative system\n"));
									return false;
								}

								if (strcmp(json_string_value(library), "mod") == 0)
								{
									json_t *mod = json_object_get(identifier, "mod");

									if (!json_is_string(mod) && !json_is_array(mod))
									{
										Global::ConfigManagerObj->ModuleConfig.append(ke::AString("\t\t\t\tMods field must be either a string or an array\n"));
										return false;
									}
								}

								json_t *value = json_object_get(identifier, "value");

								bool validValue = true;

								if (!json_is_string(value))
								{
									if (json_is_array(value))
									{
										for (size_t j = 0; j < json_array_size(value); ++j)
										{
											json_t *number = json_array_get(value, j);

											if (json_is_number(number))
											{
												if (json_number_value(number) > 0xFF || json_number_value(number) < 0x0)
												{
													validValue = false;
													break;
												}
											}
											else if (json_is_string(number))
											{
												if (strcmp(json_string_value(number), "?") != 0 && strcmp(json_string_value(number), "*") != 0)
												{
													validValue = false;
													break;
												}
											}
										}
									}
								}

								if (!validValue)
								{
									Global::ConfigManagerObj->ModuleConfig.append(ke::AString("\t\t\t\tThe value of the identifier must be a string or an array with bytes or question marks or asterisks, or an number\n"));
									return false;
								}
							}

							return true;
						}
						else
						{
							json_t *indexes = json_object_get(*root, "indexes");

							if (json_is_array(indexes))
							{
								bool libraryIsMod = (strcmp(json_string_value(library), "mod") == 0);

								for (size_t indexID = 0; indexID < json_array_size(indexes); ++indexID)
								{
									json_t *index = json_array_get(indexes, indexID);

									if (json_is_object(index))
									{
										json_t *os = json_object_get(index, "os");
										json_t *mod = json_object_get(index, "mod");
										json_t *value = json_object_get(index, "value");

										if (json_is_string(os) && (strcmp(json_string_value(os), OperativeSystems[0]) == 0 || strcmp(json_string_value(os), OperativeSystems[1]) == 0))
										{
											if (!((!libraryIsMod || json_is_string(mod)) && json_is_number(value)))
											{
												Global::ConfigManagerObj->ModuleConfig.append(ke::AString("\t\t\t\tInvalid index data\n"));
												return false;
											}
										}
										else
										{
											Global::ConfigManagerObj->ModuleConfig.append(ke::AString("\t\t\t\tInvalid index data\n"));
											return false;
										}
									}
								}
							}

							return true;
						}
					}
					else
					{
						UTIL_Format(msg, sizeof(msg) - 1, "\t\t\t\tLibrary \"%s\" is not registered\n", json_string_value(library));
						Global::ConfigManagerObj->ModuleConfig.append(ke::AString(msg));
					}
				}
				else
				{
					Global::ConfigManagerObj->ModuleConfig.append(ke::AString("\t\t\t\tThe function must have a \"library\"\n"));
				}
			}
			else
			{
				Global::ConfigManagerObj->ModuleConfig.append(ke::AString("\t\t\t\tFunction name must be equal to the filename\n"));
			}
		}
		else
		{
			Global::ConfigManagerObj->ModuleConfig.append(ke::AString("\t\t\t\tFunction name must be a string\n"));
		}
	}
	else
	{
		Global::ConfigManagerObj->ModuleConfig.append(ke::AString("\t\t\t\tFile incorrectly formatted\n"));
	}

	return false;
}

void parseFile(ke::AString folder, ke::AString filename, ke::AString classname = ke::AString(""))
{
	char name[64];

	if (classname.length())
	{
		UTIL_Format(name, sizeof(name) - 1, "%s::%s", classname.chars(), filename.chars());
	}
	else
	{
		UTIL_Format(name, sizeof(name) - 1, "%s", filename.chars());
	}

	json_t *root = NULL;
	char msg[100];
	char path[255];

	UTIL_Format(path, sizeof(path) - 1, "%s%s", folder.chars(), filename.chars());

	struct stat tempStat; stat(path, &tempStat);
	time_t newTimestamp = tempStat.st_mtime;

	time_t timestamp = Global::FunctionStructuresManagerObj->getTimestamp(name);

	if (timestamp == newTimestamp)
	{
		Global::ConfigManagerObj->ModuleConfig.append(ke::AString("\t\t\t\tFunction structure is updated\n"));
		return;
	}

	timestamp = Global::FunctionManagerObj->getTimestamp(name);

	if (timestamp == newTimestamp)
	{
		Global::ConfigManagerObj->ModuleConfig.append(ke::AString("\t\t\t\tFunction is updated\n"));
		return;
	}

	timestamp = Global::FunctionVirtualManagerObj->getTimestamp(name);

	if (timestamp == newTimestamp)
	{
		Global::ConfigManagerObj->ModuleConfig.append(ke::AString("\t\t\t\tVirtual function is updated\n"));
		return;
	}

	if (validateFile(folder, filename, classname, &root))
	{
		json_t *library = json_object_get(root, "library");
		json_t *identifiers = json_object_get(root, "identifiers");

		if (!identifiers || json_is_null(identifiers))
		{
			json_t * argumentsJson = json_object_get(root, "arguments");
			json_t * returnJson = json_object_get(root, "return");

			bool isMethod = classname.length() > 0;

			unsigned int size = json_array_size(argumentsJson) + (int)isMethod;

			TypeHandler** arguments = (TypeHandler**)malloc(sizeof(TypeHandler*) * (size));

			if (isMethod)
			{
				char classtype[32];
				UTIL_Format(classtype, sizeof(classtype) - 1, "%s *", classname.chars());

				arguments[0] = Global::TypeHandlerManagerObj->getTypeHandler(classtype);
			}

			for (unsigned int argN=(int)isMethod; argN < size; argN++)
			{
				arguments[argN] = Global::TypeHandlerManagerObj->getTypeHandler((char*)json_string_value(json_object_get(json_array_get(argumentsJson, argN - ((int)isMethod)), "type")));
			}

			TypeHandler* returnValue = NULL;

			if (json_object_size(returnJson) != 0)
			{
				returnValue = Global::TypeHandlerManagerObj->getTypeHandler((char*)json_string_value(json_object_get(returnJson, "type")));
			}

			FunctionStructure* functionStructure = new FunctionStructure;

			functionStructure->argumentsCount = size;
			functionStructure->argumentsHandlers = arguments;
			functionStructure->returnHandler = returnValue;
			functionStructure->isMethod = isMethod;
			functionStructure->address = NULL;
			functionStructure->library = json_string_value(library);
			functionStructure->name = name;

			json_t *indexes = json_object_get(root, "indexes");

			if (json_is_array(indexes))
			{
				bool libraryIsMod = (strcmp(json_string_value(library), "mod") == 0);

				for (size_t indexID = 0; indexID < json_array_size(indexes); ++indexID)
				{
					json_t *index = json_array_get(indexes, indexID);

					if (strcmp(json_string_value(json_object_get(index, "os")), OperativeSystem) == 0)
					{
						if (!libraryIsMod || Global::Modname.compare(json_string_value(json_object_get(index, "mod"))) == 0)
						{
							functionStructure->virtualTableIndex = (unsigned int)json_integer_value(json_object_get(index, "value"));
							Global::FunctionVirtualManagerObj->add(functionStructure, newTimestamp);
							return;
						}
					}
				}
			}
			else
			{
				Global::FunctionStructuresManagerObj->addFunctionStructure(functionStructure, newTimestamp);
			}
		}
		else
		{
			bool libraryIsMod = (strcmp(json_string_value(library), "mod") == 0);

			for (size_t i = 0; i < json_array_size(identifiers); ++i)
			{
				json_t *identifier = json_array_get(identifiers, i);
				json_t *os = json_object_get(identifier, "os");

				if (strcmp(json_string_value(os), OperativeSystem) == 0)
				{
					if (libraryIsMod)
					{
						json_t *mod = json_object_get(identifier, "mod");

						if (Global::Modname.compare(json_string_value(mod)) != 0)
						{
							continue;
						}
					}

					void* functionAddress = NULL;

					json_t *value = json_object_get(identifier, "value");

					if (json_is_string(value))
					{
						UTIL_Format(msg, sizeof(msg) - 1, "\t\t\t\tSearching for name \"%s\"... ", json_string_value(value));
						Global::ConfigManagerObj->ModuleConfig.append(ke::AString(msg));

						functionAddress = LibrariesManager::findFunction(json_string_value(library), json_string_value(value));
					}
					else if (json_is_array(value))
					{
						byte* signature = new byte[json_array_size(value)];
						SignatureEntryType* signatureData = new SignatureEntryType[json_array_size(value)];

						for (size_t j = 0; j < json_array_size(value); ++j)
						{
							json_t *cell = json_array_get(value, j);

							if (json_is_integer(cell))
							{
								signature[j] = (byte)json_integer_value(cell);
								signatureData[j] = SpecificByte;
							}
							else
							{
								if (strcmp(json_string_value(cell), "?") == 0)
								{
									signatureData[j] = AnyByte;
								}
								else
								{
									signatureData[j] = AnyByteOrNothing;
								}
							}
						}

						unsigned int min = json_array_size(value);

						if (min > 5)
							min = 5;

						UTIL_Format(msg, sizeof(msg) - 1, "\t\t\t\tSearching for signature \"");
						Global::ConfigManagerObj->ModuleConfig.append(ke::AString(msg));

						for (size_t j = 0; j < min; ++j)
						{
							if (signatureData[j] == AnyByte)
							{
								UTIL_Format(msg, sizeof(msg) - 1, "[?]");
							}
							else if (signatureData[j] == AnyByteOrNothing)
							{
								UTIL_Format(msg, sizeof(msg) - 1, "[*]");
							}
							else
							{
								UTIL_Format(msg, sizeof(msg) - 1, "[0x%.2x]", signature[j]);
							}

							Global::ConfigManagerObj->ModuleConfig.append(ke::AString(msg));
						}

						if (json_array_size(value) > min)
						{
							Global::ConfigManagerObj->ModuleConfig.append(ke::AString("(...)"));
						}

						Global::ConfigManagerObj->ModuleConfig.append(ke::AString("\" ... "));

						functionAddress = LibrariesManager::findFunction(json_string_value(library), signature, signatureData, json_array_size(value));

						//UTIL_Format(msg, sizeof(msg) - 1,"[0x%x]", functionAddress );
						//Global::ConfigManagerObj->ModuleConfig.append( msg );

						delete[] signature;
						delete[] signatureData;
					}
					else if (json_is_integer(value))
					{
						long offset = (long)json_integer_value(value);

						functionAddress = (void*)LibrariesManager::getAddressWithOffset(offset, json_string_value(library));

						if (!functionAddress)
						{
							UTIL_Format(msg, sizeof(msg) - 1, "\t\t\t\t Offset not contained within the library %s (0x%lx)\n", json_string_value(library), offset);
							Global::ConfigManagerObj->ModuleConfig.append(ke::AString(msg));
							break;
						}
					}

					if (functionAddress)
					{
						json_t *argumentsJson = json_object_get(root, "arguments");
						json_t *returnJson = json_object_get(root, "return");

						bool isMethod = classname.length() > 0;

						unsigned int size = json_array_size(argumentsJson) + (int)isMethod;

						TypeHandler** arguments = (TypeHandler**)malloc(sizeof(TypeHandler*) * (size));

						if (isMethod)
						{
							char classtype[32];
							UTIL_Format(classtype, sizeof(classtype) - 1, "%s *", classname.chars());

							arguments[0] = Global::TypeHandlerManagerObj->getTypeHandler(classtype);
						}

						for (unsigned int argN=(int)isMethod; argN < size; argN++)
						{
							arguments[argN] = Global::TypeHandlerManagerObj->getTypeHandler((char*)json_string_value(json_object_get(json_array_get(argumentsJson, argN - ((int)isMethod)), "type")));
						}

						TypeHandler* returnValue = NULL;

						if (json_object_size(returnJson) != 0)
						{
							returnValue = Global::TypeHandlerManagerObj->getTypeHandler((char*)json_string_value(json_object_get(returnJson, "type")));
						}

						Function* function = new Function(functionAddress, arguments, size, returnValue, ke::AString(json_string_value(library)), isMethod);

						Global::ConfigManagerObj->ModuleConfig.append(ke::AString("FOUND\n"));

						Global::FunctionManagerObj->addFunction(name, function, newTimestamp);

						break;
					}
					else
					{
						Global::ConfigManagerObj->ModuleConfig.append(ke::AString("NOT FOUND\n"));
					}
				}
			}
		}
	}
	else
	{
		Global::ConfigManagerObj->ModuleConfig.append(ke::AString("\t\t\t\tFAILED\n"));
	}
}

void ConfigManager::parseFunctionsInfo()
{
	ke::Vector<ke::AString>* files = FilesManager::getFiles(orpheuPaths.functions.chars());
	ke::Vector<ke::AString>* folders = FilesManager::getFolders(orpheuPaths.functions.chars());

	char msg[100];
	char path[256];

	for (size_t i = 0; i < files->length(); ++i)
	{
		UTIL_Format(msg, sizeof(msg) - 1, "\t\tParsing file \"%s\" started\n", files->at(i).chars());
		Global::ConfigManagerObj->ModuleConfig.append(ke::AString(msg));

		parseFile(orpheuPaths.functions, files->at(i));

		UTIL_Format(msg, sizeof(msg) - 1, "\t\tParsing file \"%s\" ended\n", files->at(i).chars());
		Global::ConfigManagerObj->ModuleConfig.append(ke::AString(msg));
	}

	for (size_t i = 0; i < folders->length(); ++i)
	{
		ke::AString classname = folders->at(i);
		UTIL_Format(path, sizeof(path) - 1, "%s%s/", orpheuPaths.functions.chars(), classname.chars());

		UTIL_Format(msg, sizeof(msg) - 1, "\t\tParsing folder \"%s\" started\n", folders->at(i).chars());
		Global::ConfigManagerObj->ModuleConfig.append(ke::AString(msg));

		ke::Vector<ke::AString>* filesInFolder = FilesManager::getFiles(path);

		for (size_t j = 0; j < filesInFolder->length(); ++j)
		{
			UTIL_Format(msg, sizeof(msg) - 1, "\t\t\tParsing file \"%s\" started\n", filesInFolder->at(j).chars());
			Global::ConfigManagerObj->ModuleConfig.append(ke::AString(msg));

			parseFile(ke::AString(path), filesInFolder->at(j), classname);

			UTIL_Format(msg, sizeof(msg) - 1, "\t\t\tParsing file \"%s\" ended\n", filesInFolder->at(j).chars());
			Global::ConfigManagerObj->ModuleConfig.append(ke::AString(msg));
		}

		UTIL_Format(msg, sizeof(msg) - 1, "\t\tParsing folder \"%s\" ended\n", folders->at(i).chars());
		Global::ConfigManagerObj->ModuleConfig.append(ke::AString(msg));
	}
}

StringHashMap<ke::Vector<char*>*>* ConfigManager::parseTypeAliasesInfo(StringHashMap<long>& typeNameToVirtualTableOffset)
{
	char msg[100];
	char path[256];

	StringHashMap<ke::Vector<char*>*>* typeAliasesInfo = new StringHashMap<ke::Vector<char*>*>();

	ke::Vector<ke::AString>* folders = FilesManager::getFolders(orpheuPaths.typeAliases.chars());

	for (size_t i = 0; i < folders->length(); ++i)
	{
		UTIL_Format(msg, sizeof(msg) - 1, "\t\tParsing folder \"%s\"\n", folders->at(i).chars());
		Global::ConfigManagerObj->ModuleConfig.append(ke::AString(msg));

		UTIL_Format(path, sizeof(path) - 1, "%s%s/data", orpheuPaths.typeAliases.chars(), folders->at(i).chars());

		json_error_t error;
		json_t *root = json_load_file(path, 0, &error);

		bool parsingSuccessful = !json_is_null(root);

		bool correctlyFormated = false;

		if (parsingSuccessful)
		{
			if (json_is_object(root))
			{
				json_t *name = json_object_get(root, "name");
				//json_t *aliases = json_object_get(root, "aliases");

				if (json_is_string(name))
				{
					json_t *virtualTableOffsets = json_object_get(root, "vtableOffsets");

					correctlyFormated = true;

					if (json_is_object(virtualTableOffsets))
					{
						json_t *value = json_object_get(virtualTableOffsets, OperativeSystem);

						if (json_is_integer(value))
						{
							typeNameToVirtualTableOffset.insert(json_string_value(name), (long)json_integer_value(value));
						}
						else
						{
							Global::ConfigManagerObj->ModuleConfig.append(ke::AString("\t\tValue must be numeric\n"));
						}
					}

					char modsDataPath[256];
					UTIL_Format(modsDataPath, sizeof(modsDataPath) - 1, "%s%s/modsData/", orpheuPaths.typeAliases.chars(), folders->at(i).chars());

					ke::Vector<ke::AString>* files = FilesManager::getFiles(modsDataPath);

					for (unsigned int fileID=0; fileID < files->length(); fileID++)
					{
						UTIL_Format(path, sizeof(path) - 1, "%s%s", modsDataPath, files->at(fileID).chars());

						json_error_t error;
						json_t *root = json_load_file(path, 0, &error);

						bool parsingSuccessful = !json_is_null(root);

						if (parsingSuccessful)
						{
							if (json_is_object(root))
							{
								json_t *modName = json_object_get(root, "name");

								if (json_is_string(modName))
								{
									if (Global::Modname.compare(json_string_value(modName)) == 0)
									{
										json_t *vtableOffsets = json_object_get(root, "vtableOffsets");

										if (json_is_object(vtableOffsets))
										{
											json_t *value = json_object_get(vtableOffsets, OperativeSystem);

											if (json_is_integer(value))
											{
												typeNameToVirtualTableOffset.insert(json_string_value(name), json_integer_value(value));
												break;
											}
										}
										else
										{
											UTIL_Format(msg, sizeof(msg) - 1, "File \"%s\" must have a \"vtableOffsets\" entry and it must be an object\n", files->at(fileID).chars());
											Global::ConfigManagerObj->ModuleConfig.append(ke::AString(msg));
										}
									}
								}
								else
								{
									UTIL_Format(msg, sizeof(msg) - 1, "File \"%s\" must have a \"name\" entry\n", files->at(fileID).chars());
									Global::ConfigManagerObj->ModuleConfig.append(ke::AString(msg));
								}
							}
							else
							{
								UTIL_Format(msg, sizeof(msg) - 1, "File \"%s\" incorrectly formatted\n", files->at(fileID).chars());
								Global::ConfigManagerObj->ModuleConfig.append(ke::AString(msg));
							}
						}
						else
						{
							UTIL_Format(msg, sizeof(msg) - 1, "File \"%s\" incorrectly formatted\n", files->at(fileID).chars());
							Global::ConfigManagerObj->ModuleConfig.append(ke::AString(msg));
						}
					}

					char aliasesPath[256];
					UTIL_Format(aliasesPath, sizeof(aliasesPath) - 1, "%s%s/aliases/", orpheuPaths.typeAliases.chars(), folders->at(i).chars());

					files = FilesManager::getFiles(aliasesPath);

					ke::Vector<char*>* aliasesForName = new ke::Vector<char*>();

					for (unsigned int fileID=0; fileID < files->length(); fileID++)
					{
						UTIL_Format(path, sizeof(path) - 1, "%s%s", aliasesPath, files->at(fileID).chars());

						json_error_t error;
						json_t *root = json_load_file(path, 0, &error);

						bool parsingSuccessful = !json_is_null(root);

						if (parsingSuccessful)
						{
							if (json_is_array(root))
							{
								json_t *alias = json_array_get(root, 0);

								if (json_is_string(alias))
								{
									char* aliasAloc = new char[json_string_length(alias) + 1];
									strcpy(aliasAloc, json_string_value(alias));
									aliasesForName->append(aliasAloc);
								}
								else
								{
									UTIL_Format(msg, sizeof(msg) - 1, "File \"%s\" incorrectly formatted\n", files->at(fileID).chars());
									Global::ConfigManagerObj->ModuleConfig.append(ke::AString(msg));
								}
							}
							else
							{
								UTIL_Format(msg, sizeof(msg) - 1, "File \"%s\" incorrectly formatted\n", files->at(fileID).chars());
								Global::ConfigManagerObj->ModuleConfig.append(ke::AString(msg));
							}
						}
						else
						{
							UTIL_Format(msg, sizeof(msg) - 1, "File \"%s\" incorrectly formatted\n", files->at(fileID).chars());
							Global::ConfigManagerObj->ModuleConfig.append(ke::AString(msg));
						}
					}

					if (aliasesForName->length())
					{
						UTIL_Format(msg, sizeof(msg) - 1, "\t\t\tAdding alias\"%s\"\n", json_string_value(name));
						Global::ConfigManagerObj->ModuleConfig.append(ke::AString(msg));

						typeAliasesInfo->insert(json_string_value(name), aliasesForName);
					}
					else
					{
						delete aliasesForName;
					}
				}
			}
		}

		if (!correctlyFormated)
			Global::ConfigManagerObj->ModuleConfig.append(ke::AString("\t\t\tFailed. File is incorrectly formated.\n"));
	}

	Global::ConfigManagerObj->ModuleConfig.append(ke::AString("\n"));

	return typeAliasesInfo;
}
StringHashMap<char*>* ConfigManager::parseExternalLibrariesInfo()
{
	StringHashMap<char*>* externalLibrariesInfo = new StringHashMap<char*>();

	ke::Vector<ke::AString>* files = FilesManager::getFiles(orpheuPaths.libraries.chars());

	char msg[100];
	char path[256];

	for (size_t i = 0; i < files->length(); ++i)
	{
		UTIL_Format(path, sizeof(path) - 1, "%s%s", orpheuPaths.libraries.chars(), files->at(i).chars());

		json_error_t error;
		json_t *root = json_load_file(path, 0, &error);

		bool parsingSuccessful = !json_is_null(root);

		UTIL_Format(msg, sizeof(msg) - 1, "\t\tParsing file \"%s\"\n", files->at(i).chars());
		Global::ConfigManagerObj->ModuleConfig.append(ke::AString(msg));

		bool correctlyFormated = false;

		if (parsingSuccessful)
		{
			if (json_is_array(root))
			{
				if (json_array_size(root) == 2)
				{
					json_t *name = json_array_get(root, 0);
					json_t *cvar = json_array_get(root, 1);

					if (json_is_string(name) && json_is_string(cvar))
					{
						correctlyFormated = true;

						char* cvarString = new char[json_string_length(cvar) + 1];
						char* nameString = new char[json_string_length(name) + 1];

						strcpy(cvarString, json_string_value(cvar));
						strcpy(nameString, json_string_value(name));

						externalLibrariesInfo->insert(cvarString, nameString);

						UTIL_Format(msg, sizeof(msg) - 1, "\t\t\tAdded library %s identified by cvar %s\n", json_string_value(name), json_string_value(cvar));
						Global::ConfigManagerObj->ModuleConfig.append(ke::AString(msg));
					}
				}
			}
		}

		if (!correctlyFormated)
		{
			Global::ConfigManagerObj->ModuleConfig.append(ke::AString("\t\t\tFailed. File is incorrectly formated.\n"));
		}
	}

	return externalLibrariesInfo;
}

void ConfigManager::parseVirtualFunctionsInfo()
{
	ke::Vector<ke::AString>* folders = FilesManager::getFolders(orpheuPaths.virtualFunctions.chars());

	char path[256];
	char msg[100];

	for (size_t i = 0; i < folders->length(); ++i)
	{
		ke::AString classname = folders->at(i);
		UTIL_Format(path, sizeof(path) - 1, "%s%s/", orpheuPaths.virtualFunctions.chars(), classname.chars());

		UTIL_Format(msg, sizeof(msg) - 1, "\t\tParsing folder \"%s\" started\n", folders->at(i).chars());
		Global::ConfigManagerObj->ModuleConfig.append(ke::AString(msg));

		ke::Vector<ke::AString>* filesInFolder = FilesManager::getFiles(path);

		for (size_t j = 0; j < filesInFolder->length(); ++j)
		{
			UTIL_Format(msg, sizeof(msg) - 1, "\t\t\tParsing file \"%s\" started\n", filesInFolder->at(j).chars());
			Global::ConfigManagerObj->ModuleConfig.append(ke::AString(msg));

			parseFile(ke::AString(path), filesInFolder->at(j), classname);

			UTIL_Format(msg, sizeof(msg) - 1, "\t\t\tParsing file \"%s\" ended\n", filesInFolder->at(j).chars());
			Global::ConfigManagerObj->ModuleConfig.append(ke::AString(msg));
		}

		UTIL_Format(msg, sizeof(msg) - 1, "\t\tParsing folder \"%s\" ended\n", folders->at(i).chars());
		Global::ConfigManagerObj->ModuleConfig.append(ke::AString(msg));
	}
}

void ConfigManager::parseMemoryObject(json_t *root)
{
	static char msg[100];

	json_t *nameValue = json_object_get(root, "name");

	if (json_is_string(nameValue))
	{
		ke::AString name = ke::AString(json_string_value(nameValue));

		UTIL_Format(msg, sizeof(msg) - 1, "\t\t\tProcessing memory structure \"%s\"\n", json_string_value(nameValue));
		Global::ConfigManagerObj->ModuleConfig.append(ke::AString(msg));

		json_t *libraryValue = json_object_get(root, "library");

		ke::AString library;

		if (json_is_string(libraryValue))
		{
			library = json_string_value(libraryValue);
		}

		bool isMod = library.compare("mod") == 0;

		json_t *type = json_object_get(root, "type");

		TypeHandler* typeHandler = NULL;

		if (json_is_string(type))
		{
			typeHandler = Global::TypeHandlerManagerObj->getTypeHandler((char*)json_string_value(type));

			if (!typeHandler)
			{
				UTIL_Format(msg, sizeof(msg) - 1, "\t\t\tMemory object identifier type \"%s\" is invalid\n", json_string_value(type));
				Global::ConfigManagerObj->ModuleConfig.append(ke::AString(msg));
				return;
			}
		}
		else
		{
			UTIL_Format(msg, sizeof(msg) - 1, "\t\t\tMemory object identifier field \"type\" must be a string\n");
			Global::ConfigManagerObj->ModuleConfig.append(ke::AString(msg));
			return;
		}

		static char memoryTypes[3][7] ={ "code", "data", "rodata" };

#if defined __linux__
		static int  memoryProtections[3] = {PROT_READ|PROT_EXEC,PROT_READ|PROT_WRITE,PROT_READ};
#endif
		json_t *memoryType = json_object_get(root, "memoryType");

		int memoryProtection = 0;

		if (json_is_string(memoryType))
		{
			bool valid = false;

			for (int i=0; i < 3; ++i)
			{
				if (!strcmp(json_string_value(memoryType), memoryTypes[i]))
				{
#if defined __linux__
					memoryProtection = memoryProtections[i];
#endif
					valid = true;
					break;
				}
			}

			if (!valid)
			{
				UTIL_Format(msg, sizeof(msg) - 1, "\t\t\tMemory object \"memoryType\" has invalid value. Valid: [\"%s\",\"%s\",\"%s\"]\n", memoryTypes[0], memoryTypes[1], memoryTypes[2]);
				Global::ConfigManagerObj->ModuleConfig.append(ke::AString(msg));
				return;
			}
		}
		else
		{
			UTIL_Format(msg, sizeof(msg) - 1, "\t\t\tMemory object \"memoryType\" field is invalid\n");
			Global::ConfigManagerObj->ModuleConfig.append(ke::AString(msg));
			return;
		}

		MemoryStructure* memoryStructure = new MemoryStructure;

		memoryStructure->library = library;
		memoryStructure->name = name;
		memoryStructure->typeHandler = typeHandler;
		memoryStructure->memoryProtection = memoryProtection;

		json_t *identifiers = json_object_get(root, "identifiers");

		if (json_is_array(identifiers))
		{
			for (size_t i = 0; i < json_array_size(identifiers); ++i)
			{
				json_t *identifier = json_array_get(identifiers, i);

				if (json_is_object(identifier))
				{
					json_t *mod = json_object_get(identifier, "mod");

					if (isMod)
					{
						if (!json_is_string(mod))
						{
							UTIL_Format(msg, sizeof(msg) - 1, "\t\t\tMemory object identifier must have a \"mod\" field\n");
							Global::ConfigManagerObj->ModuleConfig.append(ke::AString(msg));

							return;
						}
						else if (Global::Modname.compare(json_string_value(mod)) != 0)
						{
							continue;
						}
					}

					json_t *os = json_object_get(identifier, "os");

					if (!json_is_string(os))
					{
						UTIL_Format(msg, sizeof(msg) - 1, "\t\t\tMemory object identifier must have a \"os\" field\n");
						Global::ConfigManagerObj->ModuleConfig.append(ke::AString(msg));
						return;
					}
					else
					{
						if (strcmp(json_string_value(os), OperativeSystem) != 0)
						{
							continue;
						}
					}

					json_t *value = json_object_get(identifier, "value");

					bool isNumeric = json_is_integer(value);
					bool isArray = json_is_array(value);

					if (isNumeric || isArray)
					{
						if (isNumeric)
						{
							memoryStructure->type = MemTypeOffset;
							memoryStructure->offset = (long)json_integer_value(value);
						}
						else
						{
							json_t *displacement = json_object_get(identifier, "displacement");

							int displacementValue = 0;

							if (displacement)
							{
								if (json_is_integer(displacement))
								{
									displacementValue = (int)json_integer_value(displacement);
								}
								else
								{
									UTIL_Format(msg, sizeof(msg) - 1, "\t\t\t\tDisplacement must be numeric\n");
									Global::ConfigManagerObj->ModuleConfig.append(ke::AString(msg));
									return;
								}
							}

							memoryStructure->type = MemTypeSignature;
							memoryStructure->signatureLength =  json_array_size(value);;
							memoryStructure->displacement = displacementValue;

							byte* signature = new byte[json_array_size(value)];
							SignatureEntryType* signatureEntryData = new SignatureEntryType[json_array_size(value)];

							memoryStructure->signature = signature;
							memoryStructure->signatureEntryData = signatureEntryData;

							bool validSignature = true;

							for (size_t j = 0; j < json_array_size(value); ++j)
							{
								json_t *entry = json_array_get(value, j);

								if (json_is_string(entry))
								{
									if (strcmp(json_string_value(entry), "?") == 0)
									{
										signatureEntryData[j] = AnyByte;
									}
									else if (strcmp(json_string_value(entry), "*") == 0)
									{
										signatureEntryData[j] = AnyByteOrNothing;
									}
									else
									{
										validSignature = false;
										break;
									}
								}
								else if (json_is_integer(entry))
								{
									size_t entryValue = (size_t)json_integer_value(entry);

									if (entryValue <= 0xFF)
									{
										signatureEntryData[j] = SpecificByte;
										signature[j] = (byte)entryValue;
									}
									else
									{
										validSignature = false;
										break;
									}
								}
								else
								{
									validSignature = false;
									break;
								}
							}

							if (!validSignature)
							{
								UTIL_Format(msg, sizeof(msg) - 1, "\t\t\tInvalid signature data\n");
								Global::ConfigManagerObj->ModuleConfig.append(ke::AString(msg));

								delete[] signatureEntryData;
								delete[] signature;
								delete memoryStructure;

								return;
							}

							memoryStructure->basicAddress = LibrariesManager::findMemory(library.chars(), memoryStructure->signature, memoryStructure->signatureEntryData, memoryStructure->signatureLength, LibrariesManager::getLibraryAddress((char*)library.chars()));
						}

						Global::MemoryStructManagerObj->add(memoryStructure);

						UTIL_Format(msg, sizeof(msg) - 1, "\t\t\t\tAdded memory search structure \"%s\"\n", memoryStructure->name.chars());
						Global::ConfigManagerObj->ModuleConfig.append(ke::AString(msg));
						return;
					}
					else
					{
						UTIL_Format(msg, sizeof(msg) - 1, "\t\t\tMemory object identifier must have a \"value\" field that contains a signature or an offset\n");
						Global::ConfigManagerObj->ModuleConfig.append(ke::AString(msg));
						return;
					}
				}
				else
				{
					UTIL_Format(msg, sizeof(msg) - 1, "\t\t\tMemory identifiers must be objects\n");
					Global::ConfigManagerObj->ModuleConfig.append(ke::AString(msg));
				}
			}
		}
		else
		{
			memoryStructure->type = MemTypeSearch;
			Global::MemoryStructManagerObj->add(memoryStructure);

			UTIL_Format(msg, sizeof(msg) - 1, "\t\t\t\tAdded memory search structure \"%s\"\n", memoryStructure->name.chars());
			Global::ConfigManagerObj->ModuleConfig.append(ke::AString(msg));
			return;
		}
	}
	else
	{
		UTIL_Format(msg, sizeof(msg) - 1, "\t\t\tMemory object must have a name\n");
		Global::ConfigManagerObj->ModuleConfig.append(ke::AString(msg));
	}
}

static StringHashMap<time_t> memoryStructureNameToTimestamp;

void ConfigManager::loadMemoryStructures()
{
	char msg[100];
	char path[256];

	Global::ConfigManagerObj->ModuleConfig.append(ke::AString("\nOrpheu memory structures search started.\n"));

	Global::ConfigManagerObj->ModuleConfig.append(ke::AString("\n\tParsing memory structures started.\n\n"));

	ke::Vector<ke::AString>* files = FilesManager::getFiles(orpheuPaths.memory.chars());

	for (size_t i = 0; i < files->length(); ++i)
	{
		UTIL_Format(msg, sizeof(msg) - 1, "\t\tParsing memory file \"%s\"\n", files->at(i).chars());
		Global::ConfigManagerObj->ModuleConfig.append(ke::AString(msg));

		UTIL_Format(path, sizeof(path) - 1, "%s%s", orpheuPaths.memory.chars(), files->at(i).chars());

		struct stat tempStat; stat(path, &tempStat);
		time_t newTimestamp = tempStat.st_mtime;
		time_t timestamp;

		if (memoryStructureNameToTimestamp.retrieve((char*)files->at(i).chars(), &timestamp))
		{
			if (timestamp == newTimestamp)
			{
				UTIL_Format(msg, sizeof(msg) - 1, "\t\t\tFile \"%s\" is updated\n", files->at(i).chars());
				Global::ConfigManagerObj->ModuleConfig.append(ke::AString(msg));
				continue;
			}
			else
			{
				memoryStructureNameToTimestamp.replace((char*)files->at(i).chars(), newTimestamp);
			}
		}
		else
		{
			memoryStructureNameToTimestamp.insert((char*)files->at(i).chars(), newTimestamp);
		}

		json_error_t error;
		json_t *root = json_load_file(path, 0, &error);

		bool parsingSuccessful = !json_is_null(root);

		if (parsingSuccessful)
		{
			if (json_is_array(root))
			{
				for (size_t j=0; j < json_array_size(root); ++j)
				{
					this->parseMemoryObject(json_array_get(root, j));
				}
			}
			else
			{
				this->parseMemoryObject(root);
			}
		}
		else
		{
			UTIL_Format(msg, sizeof(msg) - 1, "\t\tFile incorrectly formatted\n");
			Global::ConfigManagerObj->ModuleConfig.append(ke::AString(msg));
		}
	}

	Global::ConfigManagerObj->ModuleConfig.append(ke::AString("\n\tParsing memory structures ended.\n"));

	Global::ConfigManagerObj->ModuleConfig.append(ke::AString("\nOrpheu memory structures search ended.\n"));
}
