
#include <configManager.h>
#include <amxxmodule.h>

#include <global.h>
#include <filesManager.h>

#include <iostream>
#include <ctime>

using namespace std;

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
	char modDir[32];
	char buffer[256];

	g_engfuncs.pfnGetGameDir(modDir);
	char* configsDir = LOCALINFO("amxx_configsdir");

	Global::Modname = modDir;

	UTIL_Format(buffer, sizeof(buffer) - 1, "%s/%s/%s", modDir, configsDir, Global::OrpheuFolders.config.chars());
	orpheuPaths.config = buffer;

	UTIL_Format(buffer, sizeof(buffer) - 1, "%s/%s/", orpheuPaths.config.chars(), Global::OrpheuFolders.functions.chars());
	orpheuPaths.functions = buffer;

	UTIL_Format(buffer, sizeof(buffer) - 1, "%s/%s/", orpheuPaths.config.chars(), Global::OrpheuFolders.libraries.chars());
	orpheuPaths.libraries = buffer;

	UTIL_Format(buffer, sizeof(buffer) - 1, "%s/%s/", orpheuPaths.config.chars(), Global::OrpheuFolders.typeAliases.chars());
	orpheuPaths.typeAliases = buffer;

	UTIL_Format(buffer, sizeof(buffer) - 1, "%s/%s/", orpheuPaths.config.chars(), Global::OrpheuFolders.virtualFunctions.chars());
	orpheuPaths.virtualFunctions = buffer;

	UTIL_Format(buffer, sizeof(buffer) - 1, "%s/%s/", orpheuPaths.config.chars(), Global::OrpheuFolders.mods.chars());
	orpheuPaths.mods = buffer;

	UTIL_Format(buffer, sizeof(buffer) - 1, "%s/%s/", orpheuPaths.config.chars(), Global::OrpheuFolders.memory.chars());
	orpheuPaths.mods = buffer;
}

void ConfigManager::loadFunctions()
{
	Global::ConfigManagerObj->ModuleConfig.append("\nOrpheu functions search started.\n");

	Global::ConfigManagerObj->ModuleConfig.append("\n\tParsing functions started.\n\n");

	parseFunctionsInfo();

	Global::ConfigManagerObj->ModuleConfig.append("\tParsing functions ended.\n\n");

	Global::ConfigManagerObj->ModuleConfig.append("\nOrpheu functions search ended.\n");
}

void ConfigManager::loadVirtualFunctions()
{
	Global::ConfigManagerObj->ModuleConfig.append("\nOrpheu virtual functions search started.\n");

	Global::ConfigManagerObj->ModuleConfig.append("\n\tParsing virtual functions started.\n\n");

	parseVirtualFunctionsInfo();

	Global::ConfigManagerObj->ModuleConfig.append("\n\tParsing virtual functions ended.\n");

	Global::ConfigManagerObj->ModuleConfig.append("\nOrpheu virtual functions search ended.\n");
}

void ConfigManager::parseModsInfo()
{
	char msg[100];
	char filepath[255];

	CVector<ke::AString>* files = FilesManager::getFiles(orpheuPaths.mods);

	sprintf(msg, "\t\tCurrent mod : \"%s\"\n\n", Global::Modname.chars());
	Global::ConfigManagerObj->ModuleConfig.append(msg);

	for (unsigned i=0; i < files->size(); i++)
	{
		UTIL_Format(filepath, sizeof(filepath) - 1, "%s/%s", orpheuPaths.mods.chars(), files->at(i).chars());

		sprintf(msg, "\t\tParsing mod file \"%s\"\n", files->at(i).chars());
		Global::ConfigManagerObj->ModuleConfig.append(msg);

		std::ifstream file(filepath);

		Json::Value root;
		Json::Reader reader;

		bool foundMod = false;
		bool parsingSuccessful = reader.parse(file, root);

		if (parsingSuccessful)
		{
			Json::Value name = root["name"];

			if (!name.isString())
			{
				sprintf(msg, "\t\tFile must have a \"name\" field");
				Global::ConfigManagerObj->ModuleConfig.append(msg);
			}
			else
			{
				if (Global::Modname.compare(name.asCString()) != 0)
				{
					Json::Value aliases = root["aliases"];

					if (aliases.isArray())
					{
						for (unsigned int j=0; j < aliases.size(); j++)
						{
							if (!aliases[j].isString())
							{
								sprintf(msg, "\t\tAll aliases must be strings");
								Global::ConfigManagerObj->ModuleConfig.append(msg);
								break;
							}
							if (Global::Modname.compare(aliases[j].asCString()) == 0)
							{
								sprintf(msg, "\t\t\tFound an alias.\n");
								Global::ConfigManagerObj->ModuleConfig.append(msg);
								Global::Modname = name.asCString();
								foundMod = true;
								break;
							}
						}
					}
					else
					{
						sprintf(msg, "\t\tAliases must be an array");
						Global::ConfigManagerObj->ModuleConfig.append(msg);
					}
				}
				else
				{
					sprintf(msg, "\t\t\tFound.\n");
					Global::ConfigManagerObj->ModuleConfig.append(msg);

					foundMod = true;
					Global::Modname = name.asCString();
				}

				if (foundMod)
				{
					Json::Value pev = root["pev"];

					if (pev.isObject())
					{
						Json::Value value = pev[OperativeSystem];

						if (value.isNumeric())
						{
							Global::pev = value.asInt();
						}
					}

					break;
				}
			}
		}
		else
		{
			sprintf(msg, "\t\t\tFile incorrectly formatted\"%s\"\n", files->at(i).chars());
			Global::ConfigManagerObj->ModuleConfig.append(msg);
		}
	}
}

void ConfigManager::loadBaseData()
{
	Global::ConfigManagerObj->ModuleConfig.append("\nOrpheu configuration started.\n");

	Global::ConfigManagerObj->ModuleConfig.append("\n\tParsing mods configuration started.\n\n");

	parseModsInfo();

	Global::ConfigManagerObj->ModuleConfig.append("\n\tParsing mods configuration ended.\n");

	Global::ConfigManagerObj->ModuleConfig.append("\n\tParsing libraries configuration started.\n\n");

	Global::LibrariesCvarToName = parseExternalLibrariesInfo();

	Global::ConfigManagerObj->ModuleConfig.append("\n\tParsing libraries configuration ended.\n");

	Global::ConfigManagerObj->ModuleConfig.append("\n\tParsing type aliases started.\n\n");

	KTrie<long> typeNameToVirtualTableOffset;
	KTrie<CVector<char*>*>* typeAliasesInfo = parseTypeAliasesInfo(typeNameToVirtualTableOffset);

	Global::ConfigManagerObj->ModuleConfig.append("\n\tParsing type aliases ended.\n");

	Global::TypeHandlerManagerObj = new TypeHandlerManager(typeAliasesInfo);

	for (unsigned int i=0; i < Global::TypesCount; i++)
	{
		Global::TypeHandlerManagerObj->registerTypeHandler(Global::Types[i]->getName(), Global::Types[i]);

		long* virtualTableOffsetPointer = typeNameToVirtualTableOffset.retrieve(Global::Types[i]->getName());

		if (virtualTableOffsetPointer)
		{
			long virtualTableOffset = *virtualTableOffsetPointer;
			Global::Types[i]->setVirtualTableOffset(virtualTableOffset);
		}
	}

	for (unsigned int i=0; i < Global::StructuresCount; i++)
		Global::TypeHandlerManagerObj->registerTypeHandler(Global::Structures[i]->getName(), Global::Structures[i]);

	Global::ConfigManagerObj->ModuleConfig.append("\nOrpheu configuration ended.\n");
}

bool validateFile(ke::AString folder, ke::AString filename, ke::AString classname, Json::Value& root)
{
	static char msg[100];
	char path[255];

	UTIL_Format(path, sizeof(path) - 1, "%s%s", folder, filename.chars());

	std::ifstream file(path);

	Json::Reader reader;

	bool parsingSuccessful = reader.parse(file, root);

	file.close();

	if (parsingSuccessful)
	{
		Json::Value name = root["name"];
		Json::Value classname_ = root["class"];

		if (name.isString())
		{
			if (name == filename.chars())
			{
				if (classname.length())
				{
					if (classname_.isString() && (classname_.asString() == classname.chars()))
					{
						char classtype[32];
						UTIL_Format(classtype, sizeof(classtype) - 1, "%s *", classname.chars());

						if (!Global::TypeHandlerManagerObj->typeHandlerExists(classtype))
						{
							Global::ConfigManagerObj->ModuleConfig.append("\t\t\t\tClass type not registered\n");
							return false;
						}
					}
					else
					{
						Global::ConfigManagerObj->ModuleConfig.append("\t\t\t\tClass name must be equal to the folder name\n");
						return false;
					}
				}

				Json::Value library = root["library"];

				if (library.isString())
				{
					if (LibrariesManager::hasLibrary((char*)library.asCString()))
					{
						Json::Value arguments = root["arguments"];

						if (!arguments.empty())
						{
							if (arguments.isArray())
							{
								for (unsigned int i=0; i < arguments.size(); i++)
								{
									Json::Value argument = arguments[i];
									Json::Value type = argument["type"];

									if (type.isString())
									{
										if (!Global::TypeHandlerManagerObj->typeHandlerExists((char*)type.asCString()))
										{
											sprintf(msg, "\t\t\t\tArgument has invalid type \"%s\"\n", type.asCString());
											Global::ConfigManagerObj->ModuleConfig.append(msg);
											return false;
										}
#ifndef _ERRORS_ONLY_
										else
										{
											sprintf(msg, "\t\t\t\tArgument type \"%s\" validated\n", type.asCString());
											Global::ConfigManagerObj->ModuleConfig.append(msg);
										}
#endif
									}
									else
									{
										Global::ConfigManagerObj->ModuleConfig.append("\t\t\t\tArguments must have a type and it must be a ke::AString\n");
										return false;
									}
								}
							}
							else
							{
								Global::ConfigManagerObj->ModuleConfig.append("\t\t\t\tArguments must be an array\n");
								return false;
							}
						}

						Json::Value return_ = root["return"];

						if (!return_.empty())
						{
							Json::Value type = return_["type"];

							if (type.isString())
							{
								if (!Global::TypeHandlerManagerObj->typeHandlerExists((char*)type.asCString()))
								{
									sprintf(msg, "\t\t\t\tReturn has invalid type \"%s\"\n", type.asCString());
									Global::ConfigManagerObj->ModuleConfig.append(msg);
									return false;
								}
#ifndef _ERRORS_ONLY_
								else
								{
									sprintf(msg, "\t\t\t\tReturn type \"%s\" validated\n", type.asCString());
									Global::ConfigManagerObj->ModuleConfig.append(msg);
								}
#endif
							}
							else
							{
								Global::ConfigManagerObj->ModuleConfig.append("\t\t\t\tReturn must have a type and it must be a ke::AString\n");
								return false;
							}
						}

						Json::Value identifiers = root["identifiers"];

						if (identifiers.isArray() && !identifiers.empty())
						{
							for (unsigned int i=0; i < identifiers.size(); i++)
							{
								Json::Value identifier = identifiers[i];
								Json::Value os = identifier["os"];

								if (!os.isString() || ((os != OperativeSystems[0]) && (os != OperativeSystems[1])))
								{
									sprintf(msg, "\t\t\t\tIdentifier empty or containing an invalid operative system\n");
									Global::ConfigManagerObj->ModuleConfig.append("\t\t\t\tIdentifier empty or containing an invalid operative system\n");
									return false;
								}

								if (library.asCString() == "mod")
								{
									Json::Value mod = identifier["mod"];

									if (!mod.isString())
									{
										Global::ConfigManagerObj->ModuleConfig.append("\t\t\t\tMod must be a ke::AString\n");
										return false;
									}
									else
									{
										Global::ConfigManagerObj->ModuleConfig.append("\t\t\t\tMods field must be an array\n");
										return false;
									}
								}

								Json::Value value = identifier["value"];

								bool validValue = true;

								if (!value.isString())
								{
									if (value.isArray())
									{
										for (unsigned int j=0; j < value.size(); j++)
										{
											if (value[j].isInt())
											{
												if ((value[j].asInt() > 0xFF) || (value[j].asInt() < 0x0))
												{
													validValue = false;
													break;
												}
											}
											else if (value[j].isString())
											{
												if ((value[j].asString() != "?") && (value[j].asString() != "*"))
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
									Global::ConfigManagerObj->ModuleConfig.append("\t\t\t\tThe value of the identifier must be a ke::AString or an array with bytes or question marks or asterisks, or an number\n");
									return false;
								}
							}

							return true;
						}
						else
						{
							Json::Value indexes = root["indexes"];

							if (!indexes.isNull() && indexes.isArray())
							{
								bool libraryIsMod = library.asString() == "mod";

								for (unsigned int indexID=0; indexID < indexes.size(); indexID++)
								{
									Json::Value index = indexes[indexID];

									if (index.isObject())
									{
										Json::Value os = index["os"];
										Json::Value mod = index["mod"];
										Json::Value value = index["value"];

										if (os.isString() && ((os.asString() == OperativeSystems[0]) || (os.asString() == OperativeSystems[1])))
										{
											if (!((!libraryIsMod || mod.isString()) && value.isNumeric()))
											{
												Global::ConfigManagerObj->ModuleConfig.append("\t\t\t\tInvalid index data\n");
												return false;
											}
										}
										else
										{
											Global::ConfigManagerObj->ModuleConfig.append("\t\t\t\tInvalid index data\n");
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
						sprintf(msg, "\t\t\t\tLibrary \"%s\" is not registered\n", library.asCString());
						Global::ConfigManagerObj->ModuleConfig.append(msg);
					}
				}
				else
				{
					Global::ConfigManagerObj->ModuleConfig.append("\t\t\t\tThe function must have a \"library\"\n");
				}
			}
			else
			{
				Global::ConfigManagerObj->ModuleConfig.append("\t\t\t\tFunction name must be equal to the filename\n");
			}
		}
		else
		{
			Global::ConfigManagerObj->ModuleConfig.append("\t\t\t\tFunction name must be a ke::AString\n");
		}
	}
	else
	{
		Global::ConfigManagerObj->ModuleConfig.append("\t\t\t\tFile incorrectly formatted\n");
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

	Json::Value root;
	static char msg[100];

	char filepath[256];
	UTIL_Format(filepath, sizeof(filepath) - 1, "%s%s", folder.chars(), filename.chars());

	boost::filesystem::path path(filepath);
	std::time_t newTimestamp = boost::filesystem::last_write_time(path);

	time_t timestamp = Global::FunctionStructuresManagerObj->getTimestamp(name);

	if (timestamp == newTimestamp)
	{
		Global::ConfigManagerObj->ModuleConfig.append("\t\t\t\tFunction structure is updated\n");
		return;
	}

	timestamp = Global::FunctionManagerObj->getTimestamp(name);

	if (timestamp == newTimestamp)
	{
		Global::ConfigManagerObj->ModuleConfig.append("\t\t\t\tFunction is updated\n");
		return;
	}

	timestamp = Global::FunctionVirtualManagerObj->getTimestamp(name);

	if (timestamp == newTimestamp)
	{
		Global::ConfigManagerObj->ModuleConfig.append("\t\t\t\tVirtual function is updated\n");
		return;
	}

	if (validateFile(folder, filename, classname, root))
	{
		Json::Value library = root["library"];

		Json::Value identifiers = root["identifiers"];

		if (identifiers.isNull())
		{
			Json::Value argumentsJson = root["arguments"];
			Json::Value returnJson = root["return"];

			bool isMethod = classname.length() > 0;

			unsigned int size = argumentsJson.size() + (int)isMethod;

			TypeHandler** arguments = (TypeHandler**)malloc(sizeof(TypeHandler*) * (size));

			if (isMethod)
			{
				char classtype[32];
				UTIL_Format(classtype, sizeof(classtype) - 1, "%s *", classname.chars());
				arguments[0] = Global::TypeHandlerManagerObj->getTypeHandler(classtype);
			}

			for (unsigned int argN=(int)isMethod; argN < size; argN++)
			{
				arguments[argN] = Global::TypeHandlerManagerObj->getTypeHandler((char*)argumentsJson[argN - ((int)isMethod)]["type"].asCString());
			}

			TypeHandler* returnValue = NULL;

			if (!returnJson.empty())
			{
				returnValue = Global::TypeHandlerManagerObj->getTypeHandler((char*)returnJson["type"].asCString());
			}

			FunctionStructure* functionStructure = new FunctionStructure;

			functionStructure->argumentsCount = size;
			functionStructure->argumentsHandlers = arguments;
			functionStructure->returnHandler = returnValue;
			functionStructure->isMethod = isMethod;
			functionStructure->address = NULL;
			functionStructure->library = library.asCString();
			functionStructure->name = name;

			Json::Value indexes = root["indexes"];

			if (!indexes.isNull() && indexes.isArray())
			{
				bool libraryIsMod = library.asString() == "mod";

				for (unsigned int indexID = 0; indexID < indexes.size(); indexID++)
				{
					Json::Value index = indexes[indexID];

					if (index["os"].asString() == OperativeSystem)
					{
						if (!libraryIsMod || Global::Modname.compare(index["mod"].asCString()) == 0)
						{
							functionStructure->virtualTableIndex = index["value"].asUInt();
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
			//			void* functionAddress = NULL;

			bool libraryIsMod = library.asString() == "mod";

			for (unsigned int i=0; i < identifiers.size(); i++)
			{
				Json::Value identifier = identifiers[i];
				Json::Value os = identifier["os"];

				if (os.asString() == OperativeSystem)
				{
					if (libraryIsMod)
					{
						Json::Value mod = identifier["mod"];

						if (Global::Modname.compare(mod.asCString()) != 0)
							continue;
					}

					void* functionAddress = NULL;

					Json::Value value = identifier["value"];

					if (value.isString())
					{
						sprintf(msg, "\t\t\t\tSearching for name \"%s\"... ", (char*)value.asCString());
						Global::ConfigManagerObj->ModuleConfig.append(msg);

						functionAddress = LibrariesManager::findFunction((char*)library.asCString(), (char*)value.asCString());
					}
					else if (value.isArray())
					{
						byte* signature = new byte[value.size()];
						SignatureEntryType* signatureData = new SignatureEntryType[value.size()];

						for (unsigned int j=0; j < value.size(); j++)
						{
							Json::Value cell = value[j];

							if (cell.isInt())
							{
								signature[j] = (byte)cell.asInt();
								signatureData[j] = SpecificByte;
							}
							else
							{
								if (cell.asString() == "?")
								{
									signatureData[j] = AnyByte;
								}
								else
								{
									signatureData[j] = AnyByteOrNothing;
								}
							}
						}

						unsigned int min = value.size();

						if (min > 5)
							min = 5;

						sprintf(msg, "\t\t\t\tSearching for signature \"");
						Global::ConfigManagerObj->ModuleConfig.append(msg);

						for (unsigned int j=0; j<min; j++)
						{
							if (signatureData[j] == AnyByte)
							{
								sprintf(msg, "[?]");
							}
							else if (signatureData[j] == AnyByteOrNothing)
							{
								sprintf(msg, "[*]");
							}
							else
							{
								sprintf(msg, "[0x%.2x]", signature[j]);
							}

							Global::ConfigManagerObj->ModuleConfig.append(msg);
						}

						if (value.size() > min)
						{
							Global::ConfigManagerObj->ModuleConfig.append("(...)");
						}

						Global::ConfigManagerObj->ModuleConfig.append("\" ... ");

						functionAddress = LibrariesManager::findFunction((char*)library.asCString(), signature, signatureData, value.size());

						//sprintf(msg,"[0x%x]", functionAddress );
						//Global::ConfigManagerObj->ModuleConfig.append( msg );

						delete[] signature;
						delete[] signatureData;
					}
					else if (value.isInt())
					{
						long offset = value.asUInt();

						functionAddress = (void*)LibrariesManager::getAddressWithOffset(offset, (char*)library.asCString());

						if (!functionAddress)
						{
							sprintf(msg, "\t\t\t\t Offset not contained within the library %s (0x%lx)\n", (char*)library.asCString(), offset);
							Global::ConfigManagerObj->ModuleConfig.append(msg);
							break;
						}
					}

					if (functionAddress)
					{
						Json::Value argumentsJson = root["arguments"];
						Json::Value returnJson = root["return"];

						bool isMethod = classname.length() > 0;

						unsigned int size = argumentsJson.size() + (int)isMethod;

						TypeHandler** arguments = (TypeHandler**)malloc(sizeof(TypeHandler*) * (size));

						if (isMethod)
						{
							char classtype[32];
							UTIL_Format(classtype, sizeof(classtype) - 1, "%s *", classname.chars());
							arguments[0] = Global::TypeHandlerManagerObj->getTypeHandler(classtype);
						}

						for (unsigned int argN=(int)isMethod; argN < size; argN++)
						{
							arguments[argN] = Global::TypeHandlerManagerObj->getTypeHandler((char*)argumentsJson[argN - ((int)isMethod)]["type"].asCString());
						}

						TypeHandler* returnValue = NULL;

						if (!returnJson.empty())
						{
							returnValue = Global::TypeHandlerManagerObj->getTypeHandler((char*)returnJson["type"].asCString());
						}

						Function* function = new Function(functionAddress, arguments, size, returnValue, ke::AString(library.asCString()), isMethod);

						Global::ConfigManagerObj->ModuleConfig.append("FOUND\n");

						Global::FunctionManagerObj->addFunction(name, function, newTimestamp);

						break;
					}
					else
					{
						Global::ConfigManagerObj->ModuleConfig.append("NOT FOUND\n");
					}
				}
			}
		}
	}
	else
	{
		Global::ConfigManagerObj->ModuleConfig.append("\t\t\t\tFAILED\n");
	}
}

void ConfigManager::parseFunctionsInfo()
{
	CVector<ke::AString>* files = FilesManager::getFiles(orpheuPaths.functions);
	CVector<ke::AString>* folders = FilesManager::getFolders(orpheuPaths.functions);

	static char msg[100];
	char path[256];

	for (unsigned int i=0; i < files->size(); i++)
	{
		sprintf(msg, "\t\tParsing file \"%s\" started\n", files->at(i).chars());
		Global::ConfigManagerObj->ModuleConfig.append(msg);

		parseFile(orpheuPaths.functions, files->at(i));

		sprintf(msg, "\t\tParsing file \"%s\" ended\n", files->at(i).chars());
		Global::ConfigManagerObj->ModuleConfig.append(msg);
	}
	for (unsigned int i=0; i < folders->size(); i++)
	{
		ke::AString classname = folders->at(i);
		UTIL_Format(path, sizeof(path) - 1, "%s%s/", orpheuPaths.functions.chars(), classname.chars());

		sprintf(msg, "\t\tParsing folder \"%s\" started\n", folders->at(i).chars());
		Global::ConfigManagerObj->ModuleConfig.append(msg);

		CVector<ke::AString>* filesInFolder = FilesManager::getFiles(ke::AString(path));

		for (unsigned int j=0; j < filesInFolder->size(); j++)
		{
			sprintf(msg, "\t\t\tParsing file \"%s\" started\n", filesInFolder->at(j).chars());
			Global::ConfigManagerObj->ModuleConfig.append(msg);

			parseFile(ke::AString(path), filesInFolder->at(j), classname);

			sprintf(msg, "\t\t\tParsing file \"%s\" ended\n", filesInFolder->at(j).chars());
			Global::ConfigManagerObj->ModuleConfig.append(msg);
		}

		sprintf(msg, "\t\tParsing folder \"%s\" ended\n", folders->at(i).chars());
		Global::ConfigManagerObj->ModuleConfig.append(msg);
	}
}

KTrie<CVector<char*>*>* ConfigManager::parseTypeAliasesInfo(KTrie<long>& typeNameToVirtualTableOffset)
{
	static char msg[100];
	char path[256];

	KTrie<CVector<char*>*>* typeAliasesInfo = new KTrie<CVector<char*>*>();

	CVector<ke::AString>* folders = FilesManager::getFolders(orpheuPaths.typeAliases);

	for (unsigned int i=0; i < folders->size(); i++)
	{
		sprintf(msg, "\t\tParsing folder \"%s\"\n", folders->at(i).chars());
		Global::ConfigManagerObj->ModuleConfig.append(msg);

		UTIL_Format(path, sizeof(path) - 1, "%s%s/data", orpheuPaths.typeAliases.chars(), folders->at(i).chars());
		std::ifstream file(path);

		Json::Reader reader;
		Json::Value root;

		bool parsingSuccessful = reader.parse(file, root);

		file.close();

		bool correctlyFormated = false;

		if (parsingSuccessful)
		{
			if (root.isObject())
			{
				Json::Value name = root["name"];
				Json::Value aliases = root["aliases"];

				if (name.isString())
				{
					Json::Value virtualTableOffsets = root["vtableOffsets"];

					correctlyFormated = true;

					if (!virtualTableOffsets.isNull() && virtualTableOffsets.isObject())
					{
						Json::Value value = virtualTableOffsets[OperativeSystem];

						if (value.isNumeric())
						{
							typeNameToVirtualTableOffset.insert((char*)name.asCString(), value.asUInt());
						}
						else
						{
							Global::ConfigManagerObj->ModuleConfig.append("\t\tValue must be numeric\n");
						}
					}

					char modsDataPath[256];
					UTIL_Format(modsDataPath, sizeof(modsDataPath) - 1, "%s%s/modsData/", orpheuPaths.typeAliases.chars(), folders->at(i).chars());

					CVector<ke::AString>* files = FilesManager::getFiles(ke::AString(modsDataPath));

					for (unsigned int fileID=0; fileID < files->size(); fileID++)
					{
						UTIL_Format(path, sizeof(path) - 1, "%s%s", modsDataPath, files->at(fileID).chars());
						std::ifstream file(path);

						Json::Reader reader;
						Json::Value root;

						bool parsingSuccessful = reader.parse(file, root);

						file.close();

						if (parsingSuccessful)
						{
							if (root.isObject())
							{
								Json::Value modName = root["name"];

								if (modName.isString())
								{
									if (Global::Modname.compare(modName.asCString()) == 0)
									{
										Json::Value vtableOffsets = root["vtableOffsets"];

										if (vtableOffsets.isObject())
										{
											Json::Value value = vtableOffsets[OperativeSystem];

											if (value.isNumeric())
											{
												typeNameToVirtualTableOffset.insert((char*)name.asCString(), value.asUInt());
												break;
											}
										}
										else
										{
											sprintf(msg, "File \"%s\" must have a \"vtableOffsets\" entry and it must be an object\n", files->at(fileID).chars());
											Global::ConfigManagerObj->ModuleConfig.append(msg);
										}
									}
								}
								else
								{
									sprintf(msg, "File \"%s\" must have a \"name\" entry\n", files->at(fileID).chars());
									Global::ConfigManagerObj->ModuleConfig.append(msg);
								}
							}
							else
							{
								sprintf(msg, "File \"%s\" incorrectly formatted\n", files->at(fileID).chars());
								Global::ConfigManagerObj->ModuleConfig.append(msg);
							}
						}
						else
						{
							sprintf(msg, "File \"%s\" incorrectly formatted\n", files->at(fileID).chars());
							Global::ConfigManagerObj->ModuleConfig.append(msg);
						}
					}

					char aliasesPath[256];
					UTIL_Format(aliasesPath, sizeof(aliasesPath) - 1, "%s%s/aliases/", orpheuPaths.typeAliases.chars(), folders->at(i).chars());

					files = FilesManager::getFiles(ke::AString(aliasesPath));

					CVector<char*>* aliasesForName = new CVector<char*>();

					for (unsigned int fileID=0; fileID < files->size(); fileID++)
					{
						UTIL_Format(path, sizeof(path) - 1, "%s%s", aliasesPath, files->at(fileID).chars());
						std::ifstream file(path);

						Json::Reader reader;
						Json::Value root;

						bool parsingSuccessful = reader.parse(file, root);

						file.close();

						if (parsingSuccessful)
						{
							if (root.isArray())
							{
								Json::Value alias = root[(unsigned int)0];

								if (alias.isString())
								{
									char* aliasAloc = new char[alias.asString().length() + 1];
									strcpy(aliasAloc, alias.asCString());
									aliasesForName->push_back(aliasAloc);
								}
								else
								{
									sprintf(msg, "File \"%s\" incorrectly formatted\n", files->at(fileID).chars());
									Global::ConfigManagerObj->ModuleConfig.append(msg);
								}
							}
							else
							{
								sprintf(msg, "File \"%s\" incorrectly formatted\n", files->at(fileID).chars());
								Global::ConfigManagerObj->ModuleConfig.append(msg);
							}
						}
						else
						{
							sprintf(msg, "File \"%s\" incorrectly formatted\n", files->at(fileID).chars());
							Global::ConfigManagerObj->ModuleConfig.append(msg);
						}
					}

					if (aliasesForName->size())
					{
						sprintf(msg, "\t\t\tAdding alias\"%s\"\n", name.asCString());
						Global::ConfigManagerObj->ModuleConfig.append(msg);

						typeAliasesInfo->insert(name.asCString(), aliasesForName);
					}
					else
					{
						delete aliasesForName;
					}
				}
			}
		}

		if (!correctlyFormated)
			Global::ConfigManagerObj->ModuleConfig.append("\t\t\tFailed. File is incorrectly formated.\n");
	}

	Global::ConfigManagerObj->ModuleConfig.append("\n");

	return typeAliasesInfo;
}
KTrie<char*>* ConfigManager::parseExternalLibrariesInfo()
{
	KTrie<char*>* externalLibrariesInfo = new KTrie<char*>();

	CVector<ke::AString>* files = FilesManager::getFiles(orpheuPaths.libraries);

	char path[256];

	for (unsigned int i=0; i < files->size(); i++)
	{
		UTIL_Format(path, sizeof(path) - 1, "%s%s", orpheuPaths.libraries.chars(), files->at(i).chars());
		std::ifstream file(path);

		Json::Reader reader;
		Json::Value root;

		bool parsingSuccessful = reader.parse(file, root);

		file.close();

		char* msg = new char[100];
		sprintf(msg, "\t\tParsing file \"%s\"\n", files->at(i).chars());
		Global::ConfigManagerObj->ModuleConfig.append(msg);

		bool correctlyFormated = false;

		if (parsingSuccessful)
		{
			if (root.isArray())
			{
				if (root.size() == 2)
				{
					Json::Value name = root[(unsigned int)0];
					Json::Value cvar = root[(unsigned int)1];

					if (name.isString() && cvar.isString())
					{
						correctlyFormated = true;

						char* cvarString = new char[cvar.asString().length() + 1];
						char* nameString = new char[name.asString().length() + 1];

						strcpy(cvarString, cvar.asCString());
						strcpy(nameString, name.asCString());

						externalLibrariesInfo->insert(cvarString, nameString);

						sprintf(msg, "\t\t\tAdded library %s identified by cvar %s\n", name.asCString(), cvar.asCString());
						Global::ConfigManagerObj->ModuleConfig.append(msg);
					}
				}
			}
		}

		if (!correctlyFormated)
			Global::ConfigManagerObj->ModuleConfig.append("\t\t\tFailed. File is incorrectly formated.\n");
	}

	return externalLibrariesInfo;
}

void ConfigManager::parseVirtualFunctionsInfo()
{
	CVector<ke::AString>* folders = FilesManager::getFolders(orpheuPaths.virtualFunctions);

	static char msg[100];
	char path[256];

	for (unsigned int i=0; i < folders->size(); i++)
	{
		ke::AString classname = folders->at(i);
		UTIL_Format(path, sizeof(path) - 1, "%s%s/", orpheuPaths.virtualFunctions.chars(), classname.chars());

		sprintf(msg, "\t\tParsing folder \"%s\" started\n", folders->at(i).chars());
		Global::ConfigManagerObj->ModuleConfig.append(msg);

		CVector<ke::AString>* filesInFolder = FilesManager::getFiles(ke::AString(path));

		for (unsigned int j=0; j < filesInFolder->size(); j++)
		{
			sprintf(msg, "\t\t\tParsing file \"%s\" started\n", filesInFolder->at(j).chars());
			Global::ConfigManagerObj->ModuleConfig.append(msg);

			parseFile(ke::AString(path), filesInFolder->at(j), classname);

			sprintf(msg, "\t\t\tParsing file \"%s\" ended\n", filesInFolder->at(j).chars());
			Global::ConfigManagerObj->ModuleConfig.append(msg);
		}

		sprintf(msg, "\t\tParsing folder \"%s\" ended\n", folders->at(i).chars());
		Global::ConfigManagerObj->ModuleConfig.append(msg);
	}
}

void ConfigManager::parseMemoryObject(Json::Value root)
{
	static char msg[100];

	Json::Value nameValue = root["name"];

	if (nameValue.isString())
	{
		ke::AString name;
		ke::AString library;

		name = nameValue.asCString();

		sprintf(msg, "\t\t\tProcessing memory structure \"%s\"\n", nameValue.asCString());
		Global::ConfigManagerObj->ModuleConfig.append(msg);

		Json::Value libraryValue = root["library"];

		if (libraryValue.isString())
		{
			library = libraryValue.asCString();
		}

		bool isMod = (library.compare("mod") == 0);

		Json::Value type = root["type"];

		TypeHandler* typeHandler = NULL;

		if (type.isString())
		{
			typeHandler = Global::TypeHandlerManagerObj->getTypeHandler((char*)type.asCString());

			if (!typeHandler)
			{
				sprintf(msg, "\t\t\tMemory object identifier type \"%s\" is invalid\n", type.asCString());
				Global::ConfigManagerObj->ModuleConfig.append(msg);
				return;
			}
		}
		else
		{
			sprintf(msg, "\t\t\tMemory object identifier field \"type\" must be a ke::AString\n");
			Global::ConfigManagerObj->ModuleConfig.append(msg);
			return;
		}

		static char memoryTypes[3][7] ={ "code", "data", "rodata" };

#if defined __linux__
		static int  memoryProtections[3] = {PROT_READ|PROT_EXEC,PROT_READ|PROT_WRITE,PROT_READ};
#endif
		Json::Value memoryType = root["memoryType"];

		int memoryProtection = 0;

		if (memoryType.isString())
		{
			bool valid = false;

			for (int i=0; i < 3; i++)
			{
				if (!strcmp(memoryType.asCString(), memoryTypes[i]))
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
				sprintf(msg, "\t\t\tMemory object \"memoryType\" has invalid value. Valid: [\"%s\",\"%s\",\"%s\"]\n", memoryTypes[0], memoryTypes[1], memoryTypes[2]);
				Global::ConfigManagerObj->ModuleConfig.append(msg);
				return;
			}
		}
		else
		{
			sprintf(msg, "\t\t\tMemory object \"memoryType\" field is invalid\n");
			Global::ConfigManagerObj->ModuleConfig.append(msg);
			return;
		}

		MemoryStructure* memoryStructure = new MemoryStructure;

		memoryStructure->library = library;
		memoryStructure->name = name;
		memoryStructure->typeHandler = typeHandler;
		memoryStructure->memoryProtection = memoryProtection;

		Json::Value identifiers = root["identifiers"];

		if (!identifiers.isNull() && identifiers.isArray())
		{
			for (unsigned int i=0; i < identifiers.size(); i++)
			{
				Json::Value identifier = identifiers[i];

				if (identifier.isObject())
				{
					Json::Value mod = identifier["mod"];

					if (isMod)
					{
						if (!mod.isString())
						{
							sprintf(msg, "\t\t\tMemory object identifier must have a \"mod\" field\n");
							Global::ConfigManagerObj->ModuleConfig.append(msg);

							return;
						}
						else if (Global::Modname.compare(mod.asCString()) != 0)
						{
							continue;
						}
					}

					Json::Value os = identifier["os"];

					if (!os.isString())
					{
						sprintf(msg, "\t\t\tMemory object identifier must have a \"os\" field\n");
						Global::ConfigManagerObj->ModuleConfig.append(msg);
						return;
					}
					else
					{
						if (os.asString() != OperativeSystem)
						{
							continue;
						}
					}

					Json::Value value = identifier["value"];

					bool isNumeric = value.isNumeric();
					bool isArray = value.isArray();

					if (isNumeric || isArray)
					{
						if (isNumeric)
						{
							memoryStructure->type = MemTypeOffset;
							memoryStructure->offset = value.asUInt();
						}
						else
						{
							Json::Value displacement = identifier["displacement"];

							int displacementValue = 0;

							if (!displacement.isNull())
							{
								if (displacement.isInt())
								{
									displacementValue = displacement.asInt();
								}
								else
								{
									sprintf(msg, "\t\t\t\tDisplacement must be numeric\n");
									Global::ConfigManagerObj->ModuleConfig.append(msg);
									return;
								}
							}

							memoryStructure->type = MemTypeSignature;
							memoryStructure->signatureLength = value.size();
							memoryStructure->displacement = displacementValue;

							byte* signature = new byte[value.size()];
							SignatureEntryType* signatureEntryData = new SignatureEntryType[value.size()];

							memoryStructure->signature = signature;
							memoryStructure->signatureEntryData = signatureEntryData;

							bool validSignature = true;

							for (unsigned int j=0; j < value.size(); j++)
							{
								Json::Value entry = value[j];

								if (entry.isString())
								{
									if (entry.asString() == "?")
									{
										signatureEntryData[j] = AnyByte;
									}
									else if (entry.asString() == "*")
									{
										signatureEntryData[j] = AnyByteOrNothing;
									}
									else
									{
										validSignature = false;
										break;
									}
								}
								else if (entry.isNumeric())
								{
									unsigned int entryValue = entry.asUInt();

									if ((entryValue >= 0) && (entryValue <= 0xFF))
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
								sprintf(msg, "\t\t\tInvalid signature data\n");
								Global::ConfigManagerObj->ModuleConfig.append(msg);

								delete[] signatureEntryData;
								delete[] signature;
								delete memoryStructure;

								return;
							}

							memoryStructure->basicAddress = LibrariesManager::findMemory((char*)library.chars(), memoryStructure->signature, memoryStructure->signatureEntryData, memoryStructure->signatureLength, LibrariesManager::getLibraryAddress((char*)library.chars()));
						}

						Global::MemoryStructManagerObj->add(memoryStructure);

						sprintf(msg, "\t\t\t\tAdded memory search structure \"%s\"\n", memoryStructure->name.chars());
						Global::ConfigManagerObj->ModuleConfig.append(msg);
						return;
					}
					else
					{
						sprintf(msg, "\t\t\tMemory object identifier must have a \"value\" field that contains a signature or an offset\n");
						Global::ConfigManagerObj->ModuleConfig.append(msg);
						return;
					}
				}
				else
				{
					sprintf(msg, "\t\t\tMemory identifiers must be objects\n");
					Global::ConfigManagerObj->ModuleConfig.append(msg);
				}
			}
		}
		else
		{
			memoryStructure->type = MemTypeSearch;
			Global::MemoryStructManagerObj->add(memoryStructure);

			sprintf(msg, "\t\t\t\tAdded memory search structure \"%s\"\n", memoryStructure->name.chars());
			Global::ConfigManagerObj->ModuleConfig.append(msg);
			return;
		}
	}
	else
	{
		sprintf(msg, "\t\t\tMemory object must have a name\n");
		Global::ConfigManagerObj->ModuleConfig.append(msg);
	}
}

static KTrie<time_t> memoryStructureNameToTimestamp;

void ConfigManager::loadMemoryStructures()
{
	char static msg[100];
	char path[256];

	Global::ConfigManagerObj->ModuleConfig.append("\nOrpheu memory structures search started.\n");

	Global::ConfigManagerObj->ModuleConfig.append("\n\tParsing memory structures started.\n\n");

	CVector<ke::AString>* files = FilesManager::getFiles(orpheuPaths.memory);

	for (unsigned int i=0; i < files->size(); i++)
	{
		sprintf(msg, "\t\tParsing memory file \"%s\"\n", files->at(i).chars());
		Global::ConfigManagerObj->ModuleConfig.append(msg);

		UTIL_Format(path, sizeof(path) - 1, "%s%s", orpheuPaths.memory.chars(), files->at(i).chars());

		time_t* timestampPointer = memoryStructureNameToTimestamp.retrieve((char*)files->at(i).chars());
		time_t newTimestamp = boost::filesystem::last_write_time(path);

		if (timestampPointer)
		{
			if (*timestampPointer == newTimestamp)
			{
				sprintf(msg, "\t\t\tFile \"%s\" is updated\n", files->at(i).chars());
				Global::ConfigManagerObj->ModuleConfig.append(msg);
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

		std::ifstream file(path);

		Json::Value root;
		Json::Reader reader;

		bool parsingSuccessful = reader.parse(file, root);

		if (parsingSuccessful)
		{
			if (root.isArray())
			{
				for (unsigned int j=0; j < root.size(); j++)
				{
					this->parseMemoryObject(root[j]);
				}
			}
			else
			{
				this->parseMemoryObject(root);
			}
		}
		else
		{
			sprintf(msg, "\t\tFile incorrectly formatted\n");
			Global::ConfigManagerObj->ModuleConfig.append(msg);
		}
	}

	Global::ConfigManagerObj->ModuleConfig.append("\n\tParsing memory structures ended.\n");

	Global::ConfigManagerObj->ModuleConfig.append("\nOrpheu memory structures search ended.\n");
}

size_t UTIL_Format(char *buffer, size_t maxlength, const char *fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	size_t len = vsnprintf(buffer, maxlength, fmt, ap);
	va_end(ap);

	if (len >= maxlength)
	{
		buffer[maxlength - 1] = '\0';
		return (maxlength - 1);
	}
	else
	{
		return len;
	}
}