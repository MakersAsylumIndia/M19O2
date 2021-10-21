#pragma once

#include "utility/CommandDispatcherBase.h"

template<int MAX_COMMANDS = 10, int MAX_VARIABLES = 10> class CommandDispatcher : public MLP::CommandDispatcherBase
{
  // Array of commands we can match & dispatch. 
  MLP::CommandCallback m_Commands[MAX_COMMANDS];
  
  // Array of variables we can match & set/print
  MLP::VariableMap m_Variables[MAX_VARIABLES];


public:
  CommandDispatcher() : CommandDispatcherBase(m_Commands, MAX_COMMANDS, m_Variables, MAX_VARIABLES)
  {
  }
};