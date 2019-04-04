//
// ResultSet.h
//
// ResultSet imp.
//
// Copyright (c) 2009, Renato Tegon Forti - re.tf@acm.org
//
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
// SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
// FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.
//


#include <iostream>
#include <string>
#include <vector>
#include <map>


class ResultSet
	/// ResultSet imp
{
public:

	ResultSet()
		: _current(0)
	{
	}

	void addRow(const std::vector<std::string>& row)
	{
		_resultSet.push_back(row);
	}

	bool fetch(size_t field, std::string& fieldValue)
	{
		size_t sz = _resultSet.size();

		if(sz)
		{
			if(sz > _current)
			{
				fieldValue = _resultSet[_current++][field];

				return true;
			}
		}

		_current = 0;

		return false;
	}

	bool fetch(std::vector<std::string>& rowValue)
	{
		size_t sz = _resultSet.size();

		if(sz)
		{
			if(sz > _current)
			{
				rowValue = _resultSet[_current++];

				return true;
			}
		}

		_current = 0;

		return false;
	}

	std::string get(size_t row, size_t field)
	{
		return _resultSet[row][field];
	}

	std::vector<std::string> get(size_t row)
	{
		return _resultSet[row];
	}

	size_t countRows(void)
	{
		if (_resultSet.empty()) return 0;

		return _resultSet.size();
	}

	size_t countFields(void)
	{
		if (_resultSet[0].empty()) return 0;

		return _resultSet[0].size();
	}

private:

	std::vector<std::vector<std::string> > _resultSet;

	size_t _current;

}; // ResultSet
