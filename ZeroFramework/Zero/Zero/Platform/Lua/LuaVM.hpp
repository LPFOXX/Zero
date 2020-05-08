#pragma once

#include "../../Core/Clock.h"
#include "../../Core/Events.h"

#include "../../Platform/RTTR/RttrMetadata.h"

#include <lua.hpp>

namespace zr
{
	static const std::string kLuaGlobalNamespace = "zr";
	static const std::string kLuaPrefix = "zr_lua_";
	static const std::string kLuaClassSuffix = "_Class";
	static const std::string kLuaMetatableSuffix = "_Metatable";

	class LuaVM
	{
	public:
		using LuaFunction = int(*)(lua_State*);
		using LuaMethods = std::vector< std::pair<std::string, LuaFunction>>;

	private:
		union PassByValue
		{
			bool bValue;

			char cValue;
			unsigned char ucValue;

			short sValue;
			unsigned short usValue;

			unsigned uiValue;
			int iValue;

			float fValue;
			double dValue;

			const char* strValue;
		};

		struct MethodArgs
		{
			MethodArgs(unsigned count) :
				pbv(count),
				args(count)
			{

			}

			std::vector<PassByValue> pbv;
			std::vector<rttr::argument> args;
		};

	public:
		inline LuaVM() :
			mL(nullptr),
			mHasError(false)
		{
			mL = luaL_newstate();
			if (mL != nullptr) {
				luaL_openlibs(mL);

				for (auto& method : rttr::type::get_global_methods()) {
					const char* globalMethodName = method.get_name().data();

					lua_pushstring(mL, globalMethodName);
					lua_pushcclosure(mL, &LuaVM::GlobalMethod, 1);
					lua_setglobal(mL, globalMethodName);
				}

				for (auto& type : rttr::type::get_types()) {
					const char* typeName = type.get_name().data();

					if (type.is_enumeration()) {
						rttr::enumeration& enumeration = type.get_enumeration();
						bindEnumeration(enumeration);
					}
					else if (type.is_class() && !type.is_pointer() && !type.is_wrapper()) {
						rttr::variant& var = type.get_metadata(RTTRMetadata::Bool::StaticClass);
						if (var.is_valid()) {
							bool isStaticClass = var.get_value<bool>();

							if (isStaticClass) {
								bindStaticClass(type);
							}
						}
						else {
							bindRegularClass(type);
						}
					}
				}
			}
			else {
				// issue error
				mHasError = true;
			}
		}

		inline virtual ~LuaVM()
		{
			lua_close(mL);
		}

		inline bool loadFile(const std::string& filePath)
		{
			return checkLua(luaL_dofile(mL, filePath.c_str()));
		}

		inline void executeCommand(const std::string& command)
		{
			if (this->operator bool())
				if (luaL_dostring(mL, command.c_str()) == LUA_OK) {

				}
		}

		inline void executeCommand(const char* command)
		{
			if (this->operator bool())
				luaL_dostring(mL, command);
		}

		//template <typename T>
		//inline bool createClass(const std::string& className, const LuaMethods& methods)
		//{
		//	if (this->operator bool()) {
		//		std::string classTableName;
		//		std::string classMetatableName;

		//		GetMetatableName<T>(classTableName, classMetatableName);

		//		auto constructor = [](lua_State* L) -> int {
		//			// TODO: get arguments
		//			/*int paddleSide = (int)Side::Left;
		//			if (lua_isnumber(L, 1)) {
		//				paddleSide = (int)lua_tonumber(L, 1);
		//			}*/

		//			void* ptr = lua_newuserdata(L, sizeof(T));
		//			new (ptr) T;

		//			std::string tableName, metatableName;
		//			GetMetatableName<T>(tableName, metatableName);

		//			luaL_getmetatable(L, metatableName.c_str());
		//			lua_setmetatable(L, -2);

		//			// Bind uservalue table to the metatable to store attributes on the fly
		//			lua_newtable(L);
		//			lua_setuservalue(L, -2);

		//			return 1;
		//		};

		//		auto destructor = [](lua_State* L) -> int {
		//			if (lua_isuserdata(L, 1)) {
		//				T* ptr = (T*)lua_touserdata(L, 1);
		//				ptr->~T();
		//			}

		//			return 0;
		//		};

		//		auto classNewIndex = [](lua_State* L) -> int {
		//			if (lua_isuserdata(L, 1)) {
		//				if (lua_isstring(L, 2)) {

		//					int pushedvalueType = lua_getuservalue(L, 1);
		//					lua_pushvalue(L, 2);
		//					lua_pushvalue(L, 3);

		//					lua_settable(L, -3);
		//				}
		//			}

		//			return 0;
		//		};

		//		// Sets the constructor for the "class"
		//		lua_register(mL, className.c_str(), constructor);

		//		// Creates the table of the "class"
		//		lua_newtable(mL);
		//		int tableIndex = lua_gettop(mL);
		//		lua_pushvalue(mL, -1);
		//		lua_setglobal(mL, classTableName.c_str());

		//		// Creates the metatable of the "class"
		//		int metatableExists = !luaL_newmetatable(mL, classMetatableName.c_str());

		//		// Sets methamethods for the "class"
		//		// dtor
		//		lua_pushstring(mL, "__gc");
		//		lua_pushcfunction(mL, destructor);
		//		lua_settable(mL, -3);

		//		// __index
		//		lua_pushstring(mL, "__index");
		//		lua_pushvalue(mL, tableIndex);
		//		lua_settable(mL, -3);

		//		// __newindex
		//		lua_pushstring(mL, "__newindex");
		//		lua_pushcfunction(mL, classNewIndex);
		//		lua_settable(mL, -3);

		//		// And remaining methods
		//		for (const auto& method : methods) {
		//			lua_pushcfunction(mL, method.second);
		//			lua_setfield(mL, tableIndex, method.first.c_str());
		//		}

		//		return true;
		//	}
		//	return false;
		//}

		inline void bindFunction(const std::string& luaFunction, const LuaFunction& hostFunction)
		{
			if (this->operator bool())
				lua_register(mL, luaFunction.c_str(), hostFunction);
		}

		inline operator bool() const
		{
			return mL && !mHasError;
		}

		template <typename... Args>
		inline bool runFunction(const std::string& luaFunctionName, unsigned returnArgs, const Args&... args)
		{
			if (this->operator bool()) {
				const size_t argsCount = sizeof...(args);

				lua_getglobal(mL, luaFunctionName.c_str());
				if (lua_isfunction(mL, -1)) {
					expandInParameters(args...);

					if (checkLua(lua_pcall(mL, argsCount, returnArgs, 0))) {
						if (returnArgs != 0) {
							for (int i = 1; i <= (int)returnArgs; ++i) {
								switch (lua_type(mL, -i)) {
									case LUA_TNONE:
									{
										break;
									}
									case LUA_TNIL:
									{
										std::cout << "return argument is nil\n";
										break;
									}
									case LUA_TBOOLEAN:
									{
										break;
									}
									case LUA_TLIGHTUSERDATA:
									{
										break;
									}
									case LUA_TNUMBER:
									{
										if (lua_isinteger(mL, -i)) {
											// integer
											int64_t v = (int)lua_tointeger(mL, -i);
										}
										else {
											float v = (float)lua_tonumber(mL, -i);
										}
										break;
									}
									case LUA_TSTRING:
									{
										std::string v = lua_tostring(mL, -i);
										break;
									}
									case LUA_TTABLE:
									{
										break;
									}
									case LUA_TFUNCTION:
									{
										break;
									}
									case LUA_TUSERDATA:
									{
										break;
									}
									case LUA_TTHREAD:
									{
										break;
									}
									default: break;
								}
							}
						}
					}
					else {
						return false;
					}
				}
				else {
					return false;
				}
			}
			else {
				return false;
			}
			return true;
		}

		inline bool runFunction(const std::string& luaFunctionName, unsigned returnArgs)
		{
			if (this->operator bool()) {
				lua_getglobal(mL, luaFunctionName.c_str());

				if (!checkLua(lua_pcall(mL, 0, 0, 0))) {
					return false;
				}
			}
			else {
				return false;
			}
			return true;
		}

	private:
		template <typename Arg>
		inline void expandInParameters(const Arg& arg)
		{
			rttr::type& typeOfArg = rttr::type::get<Arg>();

			if (typeOfArg.is_class()) {
				void* ptr = lua_newuserdata(mL, sizeof(rttr::variant));

				if (ptr != nullptr) {
					new (ptr) rttr::variant(&arg);

					std::string metatableName;
 					GetMetatableName(typeOfArg, metatableName);

					luaL_getmetatable(mL, metatableName.data());
					lua_setmetatable(mL, -2);

					lua_newtable(mL);
					lua_setuservalue(mL, -2);
					return;
				}
			}
			else {
				PushValueToLua(mL, arg);
			}

			// no-op
			assert(false);
		}

		template <typename Arg>
		inline void expandInParameters()
		{
			//inline void LuaVM::expandInParameters()
		}

		template <>
		inline void expandInParameters<int>(const int& arg)
		{
			if (this->operator bool())
				lua_pushnumber(mL, arg);
		}

		template <>
		inline void expandInParameters<unsigned int>(const unsigned int& arg)
		{
			if (this->operator bool())
				lua_pushnumber(mL, arg);
		}

		template <>
		inline void expandInParameters<float>(const float& arg)
		{
			if (this->operator bool())
				lua_pushnumber(mL, arg);
		}

		template <>
		inline void expandInParameters<double>(const double& arg)
		{
			if (this->operator bool())
				lua_pushnumber(mL, arg);
		}

		template <>
		inline void expandInParameters(const char* const& arg)
		{
			if (this->operator bool())
				lua_pushstring(mL, arg);
		}

		template <>
		inline void expandInParameters(const std::string& arg)
		{
			if (this->operator bool())
				lua_pushstring(mL, arg.c_str());
		}

		template <>
		inline void expandInParameters(const Event& e)
		{
			if (this->operator bool()) {
				lua_newtable(mL);
				lua_pushstring(mL, "type");
				lua_pushinteger(mL, (int)e.getType());
				lua_settable(mL, -3);

				switch (e.getType()) {
					case zr::EventType::None:
					break;
					case zr::EventType::WindowClose:
					break;
					case zr::EventType::WindowResize:
					{
						WindowResizeEvent& ev = dynamic_cast<WindowResizeEvent&>(const_cast<Event&>(e));

						lua_pushstring(mL, "height");
						lua_pushinteger(mL, ev.getHeight());
						lua_settable(mL, -3);

						lua_pushstring(mL, "width");
						lua_pushinteger(mL, ev.getWidth());
						lua_settable(mL, -3);

						break;
					}
					case zr::EventType::WindowMove:
					break;
					case zr::EventType::WindowGainedFocus:
					break;
					case zr::EventType::WindowLostFocus:
					break;
					case zr::EventType::KeyPressed:
					{
						KeyPressedEvent& ev = dynamic_cast<KeyPressedEvent&>(const_cast<Event&>(e));

						lua_pushstring(mL, "keyCode");
						lua_pushinteger(mL, ev.getKeyCode());
						lua_settable(mL, -3);

						lua_pushstring(mL, "repeatCount");
						lua_pushinteger(mL, ev.getRepeatCount());
						lua_settable(mL, -3);

						break;
					}
					case zr::EventType::KeyReleased:
					{
						KeyReleasedEvent& ev = dynamic_cast<KeyReleasedEvent&>(const_cast<Event&>(e));

						lua_pushstring(mL, "keyCode");
						lua_pushinteger(mL, ev.getKeyCode());
						lua_settable(mL, -3);
						break;
					}
					case zr::EventType::KeyTyped:
					break;
					case zr::EventType::MouseButtonPressed:
					{
						MouseButtonPressedEvent& ev = dynamic_cast<MouseButtonPressedEvent&>(const_cast<Event&>(e));

						lua_pushstring(mL, "button");
						lua_pushinteger(mL, ev.getButton());
						lua_settable(mL, -3);

						break;
					}
					case zr::EventType::MouseButtonReleased:
					{
						MouseButtonReleasedEvent& ev = dynamic_cast<MouseButtonReleasedEvent&>(const_cast<Event&>(e));

						lua_pushstring(mL, "button");
						lua_pushinteger(mL, ev.getButton());
						lua_settable(mL, -3);

						break;
					}
					case zr::EventType::MouseMove:
					{
						MouseMoveEvent& ev = dynamic_cast<MouseMoveEvent&>(const_cast<Event&>(e));
						const glm::vec2& offset = ev.GetMovementOffset();

						lua_pushstring(mL, "x");
						lua_pushnumber(mL, ev.getX());
						lua_settable(mL, -3);

						lua_pushstring(mL, "y");
						lua_pushnumber(mL, ev.getY());
						lua_settable(mL, -3);

						lua_pushstring(mL, "offsetX");
						lua_pushnumber(mL, offset.x);
						lua_settable(mL, -3);

						lua_pushstring(mL, "offsetY");
						lua_pushnumber(mL, offset.y);
						lua_settable(mL, -3);

						break;
					}
					case zr::EventType::MouseScroll:
					{
						MouseScrollEvent& ev = dynamic_cast<MouseScrollEvent&>(const_cast<Event&>(e));

						lua_pushstring(mL, "offsetX");
						lua_pushnumber(mL, ev.getXOffset());
						lua_settable(mL, -3);

						lua_pushstring(mL, "offsetY");
						lua_pushnumber(mL, ev.getYOffset());
						lua_settable(mL, -3);

						break;
					}
					case zr::EventType::ApplicationEvent:
					break;
					default:
					break;
				}
			}
		}

		template <>
		inline void expandInParameters(const Time& elapsedTime)
		{
			if (this->operator bool())
				lua_pushnumber(mL, elapsedTime.asSeconds());
		}

		template <typename Arg, typename... Rest>
		inline void expandInParameters(const Arg& arg, const Rest&... rest)
		{
			if (this->operator bool()) {
				expandInParameters(arg);

				if (sizeof...(rest) >= 1) {
					expandInParameters(rest...);
				}
			}
		}

		inline bool checkLua(int luaResult)
		{
			if (luaResult != LUA_OK) {
				std::string errorMsg = lua_tostring(mL, -1);
				std::cout << errorMsg << std::endl;
				mHasError = true;
				return false;
			}
			return true;
		}

	private:
		void bindEnumeration(const rttr::enumeration& enumeration)
		{
			const char* typeName = enumeration.get_name().data();

			lua_newtable(mL);
			lua_pushvalue(mL, -1);
			lua_setglobal(mL, typeName);

			std::string metatableName;
			GetMetatableName(enumeration, metatableName);

			luaL_newmetatable(mL, metatableName.c_str());
			{
				lua_pushstring(mL, "__index");
				lua_pushstring(mL, typeName);
				lua_pushcclosure(mL, EnumerationIndex, 1);
				lua_settable(mL, -3);

				lua_setmetatable(mL, -2);
			}
		}

		void bindStaticClass(const rttr::type& staticClass)
		{
			const char* typeName = staticClass.get_name().data();

			lua_newtable(mL);
			lua_pushvalue(mL, -1);
			lua_setglobal(mL, typeName);

			std::string internalClassMetatableName;
			GetMetatableName(staticClass, internalClassMetatableName);

			luaL_newmetatable(mL, internalClassMetatableName.c_str());
			{
				// Index
				lua_pushstring(mL, "__index");
				lua_pushstring(mL, typeName);
				lua_pushcclosure(mL, StaticClassIndex, 1);
				lua_settable(mL, -3);

				//// NewIndex
				//lua_pushstring(mL, "__newindex");
				//lua_pushcclosure(mL, StaticClassNewIndex, 0);
				//lua_settable(mL, -3);
			}

			lua_setmetatable(mL, -2);
		}

		void bindRegularClass(const rttr::type& regularClass)
		{
			const char* typeName = regularClass.get_name().data();

			// Constructor
			lua_pushstring(mL, typeName);
			lua_pushcclosure(mL, CreateObject, 1);
			lua_setglobal(mL, typeName);

			std::string internalClassMetatableName;
			GetMetatableName(regularClass, internalClassMetatableName);

			luaL_newmetatable(mL, internalClassMetatableName.c_str());
			{
				// Destructor
				lua_pushstring(mL, "__gc");
				lua_pushcclosure(mL, DestroyObject, 0);
				lua_settable(mL, -3);

				// Index
				lua_pushstring(mL, "__index");
				lua_pushstring(mL, typeName);
				lua_pushcclosure(mL, Index, 1);
				lua_settable(mL, -3);

				// NewIndex
				lua_pushstring(mL, "__newindex");
				lua_pushstring(mL, typeName);
				lua_pushcclosure(mL, NewIndex, 1);
				lua_settable(mL, -3);
				lua_pop(mL, 1);
			}
		}

	public:
		template <typename T>
		static void GetMetatableName(const T& classType, std::string& internalMetatableName)
		{
			internalMetatableName.clear();
			//const std::string& classHashCode = std::to_string(classType.get_id());
			const std::string& classHashCode = classType.get_name().to_string();
			//const std::string& classHashCode = rttr::type::get<T>().get_name();
			internalMetatableName = kLuaPrefix + classHashCode + kLuaMetatableSuffix;
		}

	private:
		static void ParseLuaArgument(lua_State* L, int paramIndex, int luaParamStackIndex, const rttr::type& argumentType, MethodArgs& outArgs)
		{
			int luaArgType = lua_type(L, luaParamStackIndex);

			switch (luaArgType) {
				case LUA_TBOOLEAN:
				{
					if (argumentType == rttr::type::get<bool>()) {
						outArgs.pbv[paramIndex].bValue = (bool)lua_toboolean(L, luaParamStackIndex);
						outArgs.args[paramIndex] = outArgs.pbv[paramIndex].bValue;
					}
					break;
				}
				case LUA_TNUMBER:
				{
					lua_Number luaNum = lua_tonumber(L, luaParamStackIndex);
					if (argumentType == rttr::type::get<short>()) {
						outArgs.pbv[paramIndex].sValue = (short)luaNum;
						outArgs.args[paramIndex] = outArgs.pbv[paramIndex].sValue;
					}
					else if (argumentType == rttr::type::get<unsigned short>()) {
						outArgs.pbv[paramIndex].usValue = (unsigned short)luaNum;
						outArgs.args[paramIndex] = outArgs.pbv[paramIndex].usValue;
					}
					else if (argumentType == rttr::type::get<int>()) {
						outArgs.pbv[paramIndex].iValue = (int)luaNum;
						outArgs.args[paramIndex] = outArgs.pbv[paramIndex].iValue;
					}
					else if (argumentType == rttr::type::get<unsigned int>()) {
						outArgs.pbv[paramIndex].uiValue = (unsigned int)luaNum;
						outArgs.args[paramIndex] = outArgs.pbv[paramIndex].uiValue;
					}
					else if (argumentType == rttr::type::get<float>()) {
						outArgs.pbv[paramIndex].fValue = (float)luaNum;
						outArgs.args[paramIndex] = outArgs.pbv[paramIndex].fValue;
					}
					else if (argumentType == rttr::type::get<double>()) {
						outArgs.pbv[paramIndex].dValue = (double)luaNum;
						outArgs.args[paramIndex] = outArgs.pbv[paramIndex].dValue;
					}
					else if (argumentType.is_enumeration()) {
						const rttr::type& underlyingType = argumentType.get_enumeration().get_underlying_type();

						if (underlyingType == rttr::type::get<unsigned int>()) {
							outArgs.pbv[paramIndex].uiValue = (unsigned int)luaNum;
							outArgs.args[paramIndex] = outArgs.pbv[paramIndex].uiValue;
						}
						else {
							luaL_error(L, "Unhandled enumeration underlying type.");
						}
					}
					else {
						luaL_error(L, "Unrecognized parameter type '%s'", argumentType.get_name().data());
					}
					break;
				}
				case LUA_TSTRING:
				{
					if (argumentType == rttr::type::get<const char*>()) {
						outArgs.pbv[paramIndex].strValue = lua_tostring(L, luaParamStackIndex);
						outArgs.args[paramIndex] = outArgs.pbv[paramIndex].strValue;
					}
					else if (argumentType == rttr::type::get<std::string>()) {
						luaL_error(L, "Param type error. Passing '%s' as argument type. Which is a C++ type. Only C types are allowed.", argumentType.get_name().data());
					}
					break;
				}
				case LUA_TNIL:
				case LUA_TLIGHTUSERDATA:
				case LUA_TTABLE:
				case LUA_TFUNCTION:
				case LUA_TUSERDATA:
				case LUA_TTHREAD:
				default:
				{
					luaL_error(L, "Don't know this lua type '%s'.", lua_typename(L, luaArgType));
					break;
				}
			}
		}


		static int PushPair(lua_State* L)
		{

		}

		static int PushValueToLua(lua_State* L, const rttr::variant& value)
		{
			if (!value) {
				return -1;
			}

			if (value.is_type<void>()) {
				return 0;
			}
			else if (value.is_type<bool>()) {
				lua_pushboolean(L, (int)value.get_value<bool>());
				return 1;
			}
			else if (value.is_type<char>()) {
				lua_pushinteger(L, (lua_Integer)value.get_value<char>());
				return 1;
			}
			else if (value.is_type<unsigned char>()) {
				lua_pushinteger(L, (lua_Integer)value.get_value<char>());
				return 1;
			}
			else if (value.is_type<int>()) {
				lua_pushinteger(L, (lua_Integer)value.get_value<int>());
				return 1;
			}
			else if (value.is_type<unsigned int>()) {
				lua_pushinteger(L, (lua_Integer)value.get_value<unsigned int>());
				return 1;
			}
			else if (value.is_type<float>()) {
				lua_pushnumber(L, (lua_Number)value.get_value<float>());
				return 1;
			}
			else if (value.is_type<double>()) {
				lua_pushnumber(L, (lua_Number)value.get_value<double>());
				return 1;
			}
			else if (value.is_type<const char*>()) {
				lua_pushstring(L, value.get_value<const char*>());
				return 1;
			}
			else if(value.is_sequential_container()){
				std::cout << "Sequencial container\n";
				const auto& view = value.create_sequential_view();

				int pushedValues = 0;
				for (const auto& elem : view) {
					pushedValues += PushValueToLua(L, elem);
				}
				return pushedValues;
			}
			else if (value.is_associative_container()) {
				std::cout << "Associative container\n";
			}
			else if (value.is_type<std::pair<float, float>>()) {
				const auto& values = value.get_value<std::pair<float, float>>();
				lua_pushnumber(L, values.first);
				lua_pushnumber(L, values.second);
				return 2;
			}

			return -1;
		}

		static int GlobalMethod(lua_State* L)
		{
			const char* methodName = lua_tostring(L, lua_upvalueindex(1));

			if (methodName != nullptr) {
				rttr::method& method = rttr::type::get_global_method(methodName);

				if (method.is_valid()) {
					rttr::variant obj = {};
					return CallMethod(L, method, obj);
				}
			}
			else {
				luaL_error(L, "Calling unbound method");
				return 0;
			}

			return 0;
		}

		static int GlobalClass(lua_State* L)
		{
			return 0;
		}

		static int CreateObject(lua_State* L)
		{
			if (lua_isstring(L, lua_upvalueindex(1))) {
				const char* typeName = lua_tostring(L, lua_upvalueindex(1));

				if (typeName != nullptr) {
					rttr::type& type = rttr::type::get_by_name(typeName);

					if (type.is_valid()) {
						std::string internalClassMetatableName;
						GetMetatableName(type, internalClassMetatableName);

						void* userDataPtr = lua_newuserdata(L, sizeof(rttr::variant));
						if (userDataPtr != nullptr) {
							new (userDataPtr) rttr::variant(type.create());

							// To store non-natve properties create in lua environment
							lua_newtable(L);
							lua_setuservalue(L, -2);

							luaL_getmetatable(L, internalClassMetatableName.c_str());
							lua_setmetatable(L, -2);
							return 1;
						}
					}
				}
			}

			return 0;
		}

		static int DestroyObject(lua_State* L)
		{
			if (lua_isuserdata(L, 1)) {
				rttr::variant& obj = *(rttr::variant*)lua_touserdata(L, 1);
				obj.~variant();
			}
			return 0;
		}

		static int Index(lua_State* L)
		{
			if (!lua_isuserdata(L, 1)) {
				luaL_error(L, "Indexing native type. User datum expected on the stack.");
				return 0;
			}

			if (!lua_isstring(L, 2)) {
				luaL_error(L, "Expected a name of a native method or property.");
				return 0;
			}

			rttr::variant& obj = *(rttr::variant*) lua_touserdata(L, 1);
			const char* memberName = lua_tostring(L, 2);

			const char* typeName = lua_tostring(L, lua_upvalueindex(1));
			rttr::type type = rttr::type::get_by_name(typeName);

			rttr::method method = type.get_method(memberName);
			if (method) {
				if (method.get_access_level() == rttr::access_levels::public_access) {
					void* methodPtr = lua_newuserdata(L, sizeof(rttr::method));
					if (methodPtr != nullptr) {
						new (methodPtr) rttr::method(method);
						lua_pushcclosure(L, CallMemberFunction, 1);
						return 1;
					}
					else {
						luaL_error(L, "Lua out of memory.");
						return 0;
					}
				}
				else {
					luaL_error(L,
						"Can't access private of protected method '%s' of '%s'.",
						memberName,
						typeName);
					return 0;
				}
			}

			rttr::property& prop = type.get_property(memberName);
			if (prop) {
				rttr::access_levels propAccessLevel = prop.get_access_level();
				if (propAccessLevel == rttr::access_levels::public_access) {
					int pushedValues = PushValueToLua(L, prop.get_value(obj));
					if (pushedValues != -1) {
						return pushedValues;
					}
					else {
						luaL_error(L,
							"Unrecognized native type '%s' of property '%s' of type '%s'.",
							prop.get_type().get_name().data(),
							memberName,
							typeName);
						return 0;
					}
				}
				else {
					luaL_error(L,
						"Can't access private of protected property '%s' of '%s'.",
						memberName,
						typeName);
					return 0;
				}
			}

			// Handling lua generated (non-native) properties
			if (lua_getuservalue(L, 1) == LUA_TTABLE) {
				lua_pushstring(L, memberName);
				lua_gettable(L, -2);
				return 1;
			}			

			luaL_error(L,
				"'%s' is nor a method or property of type '%s'.",
				memberName,
				typeName);

			return 0;
		}

		static int NewIndex(lua_State* L)
		{
			if (!lua_isstring(L, lua_upvalueindex(1))) {
				luaL_error(L, "Setting member (\"__newindex\") needs the name of the type.");
				return 0;
			}

			if (!lua_isstring(L, 2)) {
				luaL_error(L, "Can't access '%s' members for writing. (\"__newindex\")");
				return 0;
			}

			const char* typeName = lua_tostring(L, lua_upvalueindex(1));
			const char* memberName = lua_tostring(L, 2);

			if (!lua_isuserdata(L, 1)) {
				luaL_error(L,
					"Can't access '%s' for writing on member '%s'.",
					typeName,
					memberName);
				return 0;
			}

			const rttr::variant& obj = *(rttr::variant*) lua_touserdata(L, 1);
			const rttr::type& type = rttr::type::get_by_name(typeName);

			const auto& method = type.get_method(memberName);
			if (method) {
				luaL_error(L,
					"Can't assign to the native method '%s.%s'.",
					typeName,
					memberName);
				return 0;
			}

			const auto& prop = type.get_property(memberName);
			if (prop) {
				if (prop.get_access_level() == rttr::access_levels::public_access) {
					MethodArgs args(1);
					ParseLuaArgument(L, 0, 3, prop.get_type(), args);

					if (!prop.set_value(obj, args.args[0])) {
						luaL_error(L, "Can't set value.");
					}
				}
				else {
					luaL_error(L,
						"Can't access private or protected member '%s.%s' for writing.",
						typeName,
						memberName);
				}
				return 0;
			}

			if (lua_getuservalue(L, 1) == LUA_TTABLE) {
				lua_pushvalue(L, 2);
				lua_pushvalue(L, 3);
				lua_rawset(L, -3);
				return 0;
			}

			luaL_error(L,
				"'%s' doesn't have member '%s'.",
				typeName,
				memberName);

			return 0;
		}

		static int CallMemberFunction(lua_State* L)
		{
			if (lua_isuserdata(L, 1)) {
				rttr::variant& obj = *(rttr::variant*) lua_touserdata(L, 1);

				rttr::method& method = *(rttr::method*) lua_touserdata(L, lua_upvalueindex(1));
				return CallMethod(L, method, obj);
			}
			else {
				luaL_error(L, "Invoking native type. User datum expected on the stack.");
			}

			return 0;
		}

		static int CallStaticMemberFunction(lua_State* L)
		{
			if (lua_isuserdata(L, lua_upvalueindex(1))) {
				const rttr::method& method = *(rttr::method*) lua_touserdata(L, lua_upvalueindex(1));

				rttr::variant obj = {};
				return CallMethod(L, method, obj);
			}
			return 0;
		}

		static int CallMethod(lua_State* L, const rttr::method& method, rttr::variant& obj)
		{
			bool isGlobalMethod = true;
			int luaArgsOffset = 0;
			const auto& methodParamInfos = method.get_parameter_infos();

			if (lua_isuserdata(L, 1)) {
				++luaArgsOffset;
				isGlobalMethod = false;
			}

			int numLuaArgs = lua_gettop(L) - luaArgsOffset;
			int numNatArgs = (int)methodParamInfos.size();

			MethodArgs methodArgs(numLuaArgs);
			auto& paramInfoIt = methodParamInfos.cbegin();
			for (int i = 0; i < numLuaArgs; ++i, ++paramInfoIt) {
				int luaParamStackIndex = luaArgsOffset + i + 1;
				const auto& nativeArgType = paramInfoIt->get_type();

				ParseLuaArgument(L, i, luaParamStackIndex, nativeArgType, methodArgs);
			}

			unsigned defaultArgsCount = 0;
			for (; paramInfoIt != methodParamInfos.cend(); ++paramInfoIt) {
				if (paramInfoIt->has_default_value()) ++defaultArgsCount;
			}

			/*std::for_each(paramInfoIt, methodParamInfos.cend(), [&defaultArgsCount](const rttr::parameter_info& paramInfo) {
			});*/

			if (numLuaArgs + defaultArgsCount != numNatArgs) {
				luaL_error(L, "Wrong number of arguments when calling '%s': expected %d, got %d.", method.get_name().to_string().c_str(), numNatArgs, numLuaArgs);
				return 0;
			}
			else {
				const auto& result = method.invoke_variadic(obj, methodArgs.args);
				if (result.is_valid()) {
					int pushedValues = PushValueToLua(L, result);

					if (pushedValues != -1) {
						return pushedValues;
					}
					else {
						luaL_error(L, "Unrecognized return type '%s' of native method '%s'.", result.get_type().get_name().data(), method.get_name().data());
					}
				}
				else {
					luaL_error(L, "Failed to invoke native %s: '%s'",
						(isGlobalMethod ? "global function" : "method"),
						method.get_name().data());
					return 0;
				}
			}
			return 0;
		}

		static int StaticClassIndex(lua_State* L)
		{
			const char* className = lua_tostring(L, lua_upvalueindex(1));
			if (className != nullptr) {
				const rttr::type& staticClass = rttr::type::get_by_name(className);

				const char* methodName = lua_tostring(L, 2);
				if (methodName != nullptr) {
					const rttr::method& method = staticClass.get_method(methodName);

					if (method.is_valid()) {
						void* methodPtr = lua_newuserdata(L, sizeof(rttr::method));
						if (methodPtr != nullptr) {
							new (methodPtr) rttr::method(method);
							lua_pushcclosure(L, CallStaticMemberFunction, 1);
							return 1;
						}
						else {
							luaL_error(L, "Lua out of memory.");
						}
					}
					else {
						luaL_error(L,
							"Unable to invoke native method '%s' of static class '%s'.",
							methodName,
							className);
					}
				}
				else {
					luaL_error(L, "Unable to index static class '%s'.", staticClass.get_name().data());
				}
			}
			else {
				luaL_error(L, "Unable to index static class.");
			}
			return 0;
		}

		static int EnumerationIndex(lua_State* L)
		{
			const char* enumerationName = lua_tostring(L, lua_upvalueindex(1));
			const rttr::type& type = rttr::type::get_by_name(enumerationName);

			if (type.is_valid() && type.is_enumeration()) {
				const rttr::enumeration& enumeration = type.get_enumeration();

				const char* enumMember = lua_tostring(L, 2);

				if (enumMember != nullptr) {
					const rttr::variant& value = enumeration.name_to_value(enumMember);
					if (value.is_valid()) {
						const rttr::type& underlyingType = enumeration.get_underlying_type();
						lua_Integer result = (lua_Integer)-1;

						if (underlyingType == rttr::type::get<char>()) {
							result = (lua_Integer)value.get_value<char>();
						}
						else if (underlyingType == rttr::type::get<unsigned char>()) {
							result = (lua_Integer)value.get_value<unsigned char>();
						}
						else if (underlyingType == rttr::type::get<short>()) {
							result = (lua_Integer)value.get_value<short>();
						}
						else if (underlyingType == rttr::type::get<unsigned short>()) {
							result = (lua_Integer)value.get_value<unsigned short>();
						}
						else if (underlyingType == rttr::type::get<int>()) {
							result = (lua_Integer)value.get_value<int>();
						}
						else if (underlyingType == rttr::type::get<unsigned int>()) {
							result = (lua_Integer)value.get_value<unsigned int>();
						}

						if (result != -1) {
							lua_pushinteger(L, result);
							return 1;
						}
						else {
							luaL_error(L, "Unhandled underlying enumeration type '%s'.", underlyingType.get_name().data());
						}
					}
					else {
						luaL_error(L, "Enumeration '%s' doesn't have member '%s'.", enumerationName, enumMember);
					}
				}
				else {
					luaL_error(L, "Can't index enumerator '%s'.", enumerationName);
				}
			}
			return 0;
		}

	private:
		lua_State* mL;
		bool mHasError;
	};
}
