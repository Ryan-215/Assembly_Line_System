#include <string>
#include "Utilities.h"
namespace seneca {
    char Utilities::m_delimiter = ',';

    // set the field width
    void Utilities::setFieldWidth(size_t newWidth) { m_widthField = newWidth; }

    // query for current field width
    size_t Utilities::getFieldWidth() const { return m_widthField; }

    // extract token from string
    std::string Utilities::extractToken(const std::string& str, size_t& next_pos, bool& more) {
        // check for No Token in string data
        if (m_delimiter == str[next_pos]) {
            more = false;
            throw next_pos;
        }
        else {
            std::string data = str;
            // extract token
            size_t length = data.find(m_delimiter, next_pos);
            std::string token = data.substr(next_pos, length - next_pos);
            // trim spaces around a string
            token.erase(0, token.find_first_not_of(" "));
            token = token.substr(0, token.find_last_not_of(" ") + 1);

            // set the next token position and the flag for more token exsist
            if (length != std::string::npos)
                data.erase(0, length + 1);
            else
                data.clear();
            if (data.empty())
                more = false;
            else
                next_pos = length + 1;

            // update the widthField according to the maximum token
            if (token.length() > Utilities::m_widthField)
                Utilities::m_widthField = token.length();

            return token;
        }
    }

    // set the delimiter
    void Utilities::setDelimiter(char newDelimiter) { m_delimiter = newDelimiter; }

    // get the delimiter
    char Utilities::getDelimiter() { return m_delimiter; }
}