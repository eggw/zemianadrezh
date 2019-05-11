#pragma once

#include <string>
//A struct of two strings representing the
//sender and content of a chat message
struct Message {
	Message(std::string _sender = "", std::string _content = "")
	            : sender{_sender}, content{_content} {
	}
	std::string sender;
	std::string content;
};
