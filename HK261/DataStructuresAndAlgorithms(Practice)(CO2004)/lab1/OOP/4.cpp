#include <bits/stdc++.h>

class Book
{
private:
  char* title;
  char* authors;
  int publishingYear;

  static char* allocateString(const char* str)
  {
    if (str == nullptr) return nullptr;

    char* newStr = new char[strlen(str) + 1];
    strcpy(newStr, str);
    return newStr;
  }

public:
  Book() : title(nullptr), authors(nullptr), publishingYear(0)
  {
  }

  Book(const char* title, const char* authors, int publishingYear) : publishingYear(publishingYear)
  {
    this->title = allocateString(title);
    this->authors = allocateString(authors);
  }

  Book(const Book& book) : publishingYear(book.publishingYear)
  {
    this->title = allocateString(book.title);
    this->authors = allocateString(book.authors);
  }

  void setTitle(const char* title)
  {
    this->title = allocateString(title);
  }

  void setAuthors(const char* authors)
  {
    this->authors = allocateString(authors);
  }

  void setPublishingYear(int publishingYear)
  {
    this->publishingYear = publishingYear;
  }

  char* getTitle() const
  {
    return title;
  }

  char* getAuthors() const
  {
    return authors;
  }

  int getPublishingYear() const
  {
    return publishingYear;
  }

  ~Book()
  {
    delete[] title;
    delete[] authors;

    title = authors = nullptr;
  }

  void printBook()
  {
    printf("%s\n%s\n%d", this->title, this->authors, this->publishingYear);
  }
};
