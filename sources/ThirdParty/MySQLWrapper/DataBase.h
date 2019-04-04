//
// DataBase.h
//
// DataBase common interface.
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


template<class T>
class DataBase
	/// DataBase common interface class
{
public:
	DataBase()
		: _connected(false)
	{
	}

	DataBase(const std::string& server, const std::string& user, const std::string& password, const std::string& database)
		: _connected(false)
	{
		connect(server, user, password, database);
	}
	
	virtual ~DataBase()
	{
		if(_connected)
			_dataBaseEngine.close();
	}
	
	void connect(const std::string& server, const std::string& user, const std::string& password, const std::string& database)
	{
		_dataBaseEngine.connect(server, user, password, database);

		_connected = true;
	}

	DataBase& operator<< (const std::string& sql)
	{
		_dataBaseEngine.execute(sql);

		return *this;
	}

	DataBase& operator, (ResultSet& rs)
	{
		_dataBaseEngine.populate(rs);

		return *this;
	}

private:
	T		_dataBaseEngine;
	bool	_connected;

}; // DataBase
