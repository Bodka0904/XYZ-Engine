#pragma once
#include <tuple>

namespace XYZ {

	class CommandI
	{
	public:
		virtual void Execute() = 0;

	};

	template <typename ...Args>
	class Command : public CommandI
	{
	public:
		Command(void(*func)(Args...), Args&& ... args)
		{
			m_Func = func;
			m_Args = std::forward_as_tuple(args...);
		}

		virtual void Execute() override
		{
			std::apply(m_Func, m_Args);
		}

	private:
		void(*m_Func)(Args...);
		std::tuple<Args...> m_Args;
	};

	class RenderCommandQueue
	{
	public:
		RenderCommandQueue();

		template<typename...Args>
		void Allocate(Command<Args...> cmd)
		{
			unsigned int size = sizeof(cmd);
			*(int*)m_CommandBufferPtr = size;
			m_CommandBufferPtr += sizeof(unsigned int);

			//  copy command
			memcpy(m_CommandBufferPtr, &cmd, size);
			m_CommandBufferPtr += size;

			m_CommandCount++;
		}
		void Execute();
		
	private:
		unsigned char* m_CommandBuffer;
		unsigned char* m_CommandBufferPtr;
		unsigned int m_CommandCount = 0;
		
	};
}