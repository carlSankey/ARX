#include "InputQueue.h"
#include <deque>

namespace arx {

namespace {
    std::deque<std::string> g_queue;
    std::deque<std::string> g_textQueue;
}

void inputPush(const std::string& key)
{
    if (!key.empty()) {
        g_queue.push_back(key);
    }
}

std::string inputPoll()
{
    if (g_queue.empty()) {
        return "";
    }
    std::string key = g_queue.front();
    g_queue.pop_front();
    return key;
}

std::string inputPeek()
{
    if (g_queue.empty()) {
        return "";
    }
    return g_queue.front();
}

void inputClear()
{
    g_queue.clear();
}

// --- Text input queue ---

void textPush(const std::string& ch)
{
    if (!ch.empty()) {
        g_textQueue.push_back(ch);
    }
}

std::string textPoll()
{
    if (g_textQueue.empty()) {
        return "";
    }
    std::string ch = g_textQueue.front();
    g_textQueue.pop_front();
    return ch;
}

void textClear()
{
    g_textQueue.clear();
}

} // namespace arx
