#pragma once
#include "Event.h"

namespace XYZ {

	typedef std::function<void(event_ptr)> handlerPtr;
	typedef std::map<EventType, std::vector<handlerPtr>> handler_map;

	class EventManager
	{
	public:
		EventManager() : m_Handlers(std::map<EventType, std::vector<handlerPtr>>())
		{ }

		~EventManager()
		{}

		bool FireEvent(event_ptr event);
		bool AddHandler(EventType type, handlerPtr eventHandler);
		bool RemoveHandler(EventType type, handlerPtr eventHandler);
		void RemoveAllHandlers(EventType type);


		static EventManager& Get() { return s_Instance; }
	private:
		template<typename T, typename... U>
		size_t getAddress(std::function<T(U...)> f) 
		{
			typedef T(fnType)(U...);
			fnType** fnPointer = f.template target<fnType*>();
			return (size_t)* fnPointer;
		}
	
	private:
		handler_map m_Handlers;
		static EventManager s_Instance;
	};

}
