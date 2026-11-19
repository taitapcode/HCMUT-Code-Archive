#include <algorithm>
#include <string>
#include <vector>

int longestSublist(std::vector<std::string>& words)
{
  if (words.empty()) return 0;

  int maxLen = 1, currLen = 1;

  for (std::size_t i = 1; i < words.size(); i++)
  {
    if (!words[i].empty() && !words[i - 1].empty() && words[i][0] == words[i - 1][0])
      currLen++;
    else
      currLen = 1;

    maxLen = std::max(currLen, maxLen);
  }

  return maxLen;
}
