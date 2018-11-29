#include "ChatBoxMessage.h"

ChatBoxMessage::ChatBoxMessage(std::string _sender,
                               std::string _message,
                               const sf::Font& _font,
                               unsigned int _charSize,
                               unsigned int _numberOfLines)
    :m_text{ },
     m_senderLength{_sender.length() },
     m_numberOfLines{_numberOfLines } {

    m_text.setFont(_font);
    m_text.setCharacterSize(_charSize);

    std::string finalStr{_message };
    if(_sender != "") {
        finalStr = ("<" + _sender + "> " + finalStr); }
    m_text.setString(finalStr);

    if(_sender == "Server") {
        m_text.setFillColor(sf::Color::Red, 1, _sender.length()); }
    else if(_sender == "LocalPlayer") {
        m_text.setFillColor(sf::Color::Cyan, 1, _sender.length()); }
    else {
        m_text.setFillColor(sf::Color::Magenta, 1, _sender.length()); }

    int beginPos(_sender.length() + 2);
    int endPos(finalStr.length() - 1);

    m_text.setFillColor(sf::Color::Yellow, beginPos, endPos);
    m_text.setOutlineThickness(1, beginPos, endPos);

}
