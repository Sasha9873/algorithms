#include "str.hpp"
#include <cstring>
#include <algorithm>

String::String(size_t size, const char sym) : size_(size), capacity_(size)
{
	if(capacity_ == 0) 
	{
		data_ = nullptr;
		return;
	}

	data_ = new char[capacity_ + 1];
	memset(data_, sym, size_);

	data_[size_] = '\0';
}

String::String(const char* str) : size_(strlen(str)), capacity_(strlen(str))
{
	if(capacity_ == 0) 
	{
		data_ = nullptr;
		return;
	}

	data_ = new char[capacity_ + 1];
	memcpy(data_, str, size_);

	data_[size_] = '\0';
}

String::String(const char* str, size_t size) : size_(size), capacity_(size)
{
	if(capacity_ == 0) 
	{
		data_ = nullptr;
		return;
	}

	data_ = new char[capacity_ + 1];
	memcpy(data_, str, size_);

	data_[size_] = '\0';
}

String::String(const String& str) : size_(str.size_), capacity_(str.capacity_) 
{	
	if(capacity_ == 0) 
	{
		data_ = nullptr;
		return;
	}

	data_ = new char[capacity_ + 1];
	memcpy(data_, str.data_, size_);

	data_[size_] = '\0';
}

String::~String() {
	delete[] data_;
	data_ = nullptr;
}

bool String::Empty() const
{
	return !size_;
}

size_t String::Size() const
{
	return size_;
}

size_t String::Length() const
{
	return size_;
}

size_t String::Capacity() const
{
	return capacity_;
}

const char* String::CStr() const
{
	return data_;
}

const char* String::Data() const
{
	return data_;
}

String& String::operator=(const String& str)
{
	if(this == std::addressof(str))
		return *this;

	delete[] data_;
	data_ = nullptr;

	size_ = str.size_;
	capacity_ = str.capacity_;

	if(capacity_ == 0)
		return *this;

	data_ = new char[capacity_ + 1];

	memcpy(data_, str.data_, size_);
	data_[size_] = '\0';

	return *this;
}

String& String::operator+=(const String& str)
{
	String new_str = String(size_ + str.size_, '\0');

	if(data_ != nullptr)
		memcpy(new_str.data_, data_, size_);
	if(str.data_ != nullptr)
		memcpy(new_str.data_ + size_, str.data_, str.size_);

	*this = new_str;

	return *this;
}


char& String::operator[](size_t pos) 
{
	return data_[pos];
}

const char& String::operator[](size_t pos) const 
{
	return data_[pos];
}

char& String::Front()
{
	return data_[0];
}

const char& String::Front() const
{
	return data_[0];
}

char& String::Back()
{
	return data_[size_ - 1];
}

const char& String::Back() const
{
	return data_[size_ - 1];
}

void String::Swap(String& other)
{
	std::swap(data_, other.data_);
	std::swap(size_, other.size_);
	std::swap(capacity_, other.capacity_);
}


void String::Reserve(size_t new_capacity)
{
	if(capacity_ >= new_capacity)
		return;

	char* new_data = new char[new_capacity + 1];
	if(data_ != nullptr)
	{
		memcpy(new_data, data_, size_);
		delete[] data_;
	}

	new_data[size_] = '\0';

	capacity_ = new_capacity;

	data_ = new_data;
	new_data = nullptr;
}

void String::PushBack(const char symbol)
{
	if(capacity_ == 0)
		Reserve(1);

	else if(size_ + 1 > capacity_)
		Reserve(capacity_ * 2);


	data_[size_++] = symbol;
	data_[size_] = '\0';
}


bool String::operator==(const String& other) const
{
	return !strcmp(data_, other.data_);
}



/*int operator <=> (const String& str1, const String& str2)
{
	size_t size1 = str1.Size();
	size_t size2 = str2.Size();
	size_t min_size = (size1 < size2) ? size1 : size2;

	if(min_size == 0) 
		return size1 - size2;

	int balance = memcmp(str1.data_, str2.data_, min_size);
	if(balance)
		return balance;

	return size1 - size2;
}*/



const String operator+(const String& left, const String& right)
{
	String copy(left);
	copy += right;

	return copy;
}

void String::Clear()
{
	size_ = 0;
}

void String::PopBack()
{
	if(size_ == 0)
		throw std::runtime_error("size == 0");

	--size_;
	data_[size_] = '\0';
}

void String::Resize(size_t new_size, const char symbol)
{
	if(new_size <= size_)
	{
		size_ = new_size;
		return;
	}

	Reserve(new_size);

	memset(data_ + size_, symbol, new_size - size_);
	size_ = new_size;
}


void String::ShrinkToFit() 
{
	char* new_data = new char[size_];
	if(data_ != nullptr)
	{
		memcpy(new_data, data_, size_);
		delete[] data_;
		data_ = new_data;
	}
	else
		data_ = new_data;

	capacity_ = size_;
}

std::istream& operator>>(std::istream& in, String& str) 
{
   	char new_symbol;
   	in >> new_symbol;

   	while(new_symbol != '\n')
   	{
   		str.PushBack(new_symbol);
   		in >> new_symbol;
   	}

   	str.data_[str.size_] = '\0';

    return in;
}


std::ostream& operator<<(std::ostream& out, const String& str)
{
	for(size_t i = 0; i < str.size_; ++i)
		out << str.data_[i];	

	return out;
}
