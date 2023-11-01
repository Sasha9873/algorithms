#pragma once 
#include <cstddef>
#include <istream>
#include <fstream>

class String{
public:
	String() = default;
	String(size_t size, const char sym);
	String(const char* str);
	String(const char* str, size_t size);
	String(const String& str);

	String& operator=(const String& str);
	String& operator+=(const String& str);
	bool operator==(const String& other) const;
	//friend int operator <=> (const String& str1, const String& str2);
	
	char& operator[](size_t pos);
	const char& operator[](size_t pos) const;

	bool Empty() const;
	size_t Size() const;
	size_t Length() const;
	size_t Capacity() const;
	const char* CStr() const;
	const char* Data() const;

	char& Front();
	const char& Front() const;
	char& Back();
	const char& Back() const;

	void Swap(String& other);

	void PushBack(const char symbol);
	void PopBack();
	void Reserve(size_t new_capacity);
	void Clear();
	void Resize(size_t new_size, const char symbol);
	void ShrinkToFit();

	friend std::istream& operator>>(std::istream& in, String& str);
	friend std::ostream& operator<<(std::ostream& out, const String& str);

	~String();

private:
	size_t capacity_ = 0;  // max available size of str now
	size_t size_ = 0; // current size of str
	char* data_ = nullptr;
};


const String operator+(const String& left, const String& right);