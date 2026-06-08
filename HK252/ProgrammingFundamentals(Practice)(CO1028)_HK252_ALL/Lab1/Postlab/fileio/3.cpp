#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <sstream>
using namespace std;

struct Book
{
  string author;
  vector<string> libraries;
};

void manage(string library, string book, string author)
{
  int n;
  string line;
  map<int, Book> books;
  ifstream libinput(library), authorinput(author);

  // Handle author inputs
  authorinput >> n;
  authorinput.ignore(1000, '\n');

  while(n--)
  {
    getline(authorinput, line);
    stringstream ss(line);

    if (line.empty()) continue;

    int id;
    string name;

    ss >> name;
    while(ss >> id) books[id].author = name;
  }

  // Handle library inputs
  libinput >> n;
  libinput.ignore(1000, '\n');

  while(n--)
  {
    string lib;
    int b;
    libinput >> lib;
    for(int i = 0; i < 5; i++)
    {
      libinput >> b;
      books[b].libraries.push_back(lib);
    }
  }

  libinput.close();
  authorinput.close();

  string iLib, iAuthor;
  cin >> iLib >> iAuthor;

  for(auto &x: books)
    if (x.second.author == iAuthor)
      for(string &l: x.second.libraries)
        if (l == iLib)
        {
          cout << "True";
          return;
        }

  cout << "False";
}

int main()
{
  manage("l.txt", "", "a.txt");

  return 0;
}
