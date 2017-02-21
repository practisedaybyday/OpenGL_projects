/*! @file rx_atom_ini.h
	
	@brief �ݒ�t�@�C��
 
	@author Makoto Fujisawa
	@date 2009-04
*/

#ifndef _RX_ATOM_INI_H_
#define _RX_ATOM_INI_H_


//-----------------------------------------------------------------------------
// �C���N���[�h�t�@�C��
//-----------------------------------------------------------------------------
#include <iostream>
#include <fstream>

#include <vector>
#include <string>
#include <map>
#include <algorithm>

//#include <boost/typeof/typeof.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/function.hpp>


using namespace std;


//-----------------------------------------------------------------------------
// �ݒ�t�@�C�����ڏ��
//-----------------------------------------------------------------------------
struct rxINIPair
{
	string Header;		//!< �w�b�_([Header])
	string Name, Value;	//!< ���O�ƒl(Name=Value)
	string Type;
	void* Var;
};

#define RX_INIFOR(x, y) for(vector<rxINIPair>::iterator x = y.begin(); x < y.end(); ++x)


//-----------------------------------------------------------------------------
// �N���X�̐錾
//-----------------------------------------------------------------------------
class rxINI
{
	//typedef void (*RX_HEADER_FUNC)(string*, string*, int, string);
	typedef boost::function<void (string*, string*, int, string)> RX_HEADER_FUNC;

protected:
	vector<rxINIPair> m_vContainer;
	string m_strPath;

	//! �w�b�_���Ɗ֐����֘A�Â���}�b�v
	map<string, RX_HEADER_FUNC> m_mapHeaderFunc;

public:
	//! �f�t�H���g�R���X�g���N�^
	rxINI(){}

	/*!
	 * �R���X�g���N�^
	 * @param[in] path �ݒ�t�@�C���p�X
	 */
	rxINI(const string &path)
	{
		Load(path);
	}

	//! �f�X�g���N�^
	~rxINI(){}

	/*!
	 * INI���ڂ̍쐬
	 * @param[in] header ���ڃw�b�_
	 * @param[in] name ���ږ�
	 * @param[in] value ���ڂ̒l
	 * @return INI����
	 */
	rxINIPair CreatePair(const string& header = "", const string& name = "", const string& value = "")
	{
		rxINIPair p = {header, name, value, "", NULL};
		return p;
	}

	/*!
	 * ���ڂ̓o�^
	 *  - �e���ڂƕϐ�����Έ�őΉ�����
	 * @param[in] header �w�b�_��
	 * @param[in] name ���ږ�
	 * @param[in] var ���ڂɑΉ�����l(�ϐ�)
	 * @param[in] def �l�̃f�t�H���g�l(���ڂ�������Ȃ��������������)
	 */
	template<class T> 
	void Set(const string& header, const string& name, T* var, const T def)
	{
		string type = typeid(T).name();
		string value = boost::lexical_cast<std::string>(*var);

		RX_INIFOR(i, m_vContainer){
			if(i->Name == name && i->Header == header){
				cout << name << "had been registered." << endl;
				return;
			}
		}

		*var = def;

		rxINIPair pr = {header, name, value, type, (void*)var};
		m_vContainer.push_back(pr);
	}

	/*!
	 * �w�b�_�֐��̓o�^
	 *  - �w�b�_������������w�b�_���̍��ڂ����ׂēǂݍ��݁C����������Ƃ��Ċ֐����Ăяo��
	 * @param[in] header �w�b�_��
	 * @param[in] hfunc �w�b�_�֐��|�C���^
	 */
	// ���ڂ̓o�^
	void SetHeaderFunc(string header, RX_HEADER_FUNC hfunc)
	{
		string::size_type breakpoint = 0;
		while((breakpoint = header.find(" ")) != string::size_type(-1)){
			header.erase(breakpoint, 1);
		}
		m_mapHeaderFunc[header] = hfunc;
	}


	// �ݒ�t�@�C�����ڂ�string value���X�V
	int UpdateValues(void);

	// ���O����l���擾
	string GetValueByName(const string& name);

	// �l���疼�O���擾
	string GetNameByValue(const string& value);

	// ���O����w�b�_���擾
	string GetHeaderByName(const string& name);

	// �l����w�b�_���擾
	string GetHeaderByValue(const string& value);

	// �ݒ�t�@�C���ǂݍ��݁E��������
	int Load(const string& path);
	int Save(const string& path = "");

	// ���ڃ��X�g���w�肵�Đݒ�ۑ�
	int SaveList(vector<rxINIPair> *pairs, const string& path = "");
	int SaveList(rxINIPair *pairs, unsigned int size, const string& path = "");
};



/*!
 * ���O����l���擾
 * @param[in] name ���ږ�
 * @return ���ڂ̒l(������Ȃ��������string""��Ԃ�)
 */
inline string rxINI::GetValueByName(const string& name)
{
	for(vector<rxINIPair>::iterator i = m_vContainer.begin(); i < m_vContainer.end(); ++i)
		if(i->Name == name)
			return i->Value;

	// This value was not found.
	return "";
}

/*!
 * �l���疼�O���擾
 * @param[in] value ���ڂ̒l
 * @return ���ږ�(������Ȃ��������string""��Ԃ�)
 */
inline string rxINI::GetNameByValue(const string& value)
{
	for(vector<rxINIPair>::iterator i = m_vContainer.begin(); i < m_vContainer.end(); ++i)
		if(i->Value == value)
			return i->Name;

	return "";
}

/*!
 * ���O����w�b�_���擾
 * @param[in] name ���ږ�
 * @return �w�b�_��(������Ȃ��������string""��Ԃ�)
 */
inline string rxINI::GetHeaderByName(const string& name)
{
	for(vector<rxINIPair>::iterator i = m_vContainer.begin(); i < m_vContainer.end(); ++i)
		if(i->Name == name)
			return i->Header;

	return "";
}

/*!
 * �l����w�b�_���擾
 * @param[in] value ���ڂ̒l
 * @return �w�b�_��(������Ȃ��������string""��Ԃ�)
 */
inline string rxINI::GetHeaderByValue(const string& value)
{
	for(vector<rxINIPair>::iterator i = m_vContainer.begin(); i < m_vContainer.end(); ++i)
		if(i->Value == value)
			return i->Header;

	return "";
}


/*!
 * �ݒ�t�@�C������f�[�^�擾
 * @param[in] path �ݒ�t�@�C���p�X
 * @return 
 */
inline int rxINI::Load(const string& path)
{
	ifstream in(path.c_str(), ios::in);

	if(!in || !in.is_open() || in.bad() || in.fail()){
		cout << "[rxINI::Load] Invalid or corrupted file specified" << endl;
		return 0;
	}

	m_strPath = path;

	string buf;
	string cur_header;
	string name = "", value = "";

	string::size_type header_end = 0;
	string::size_type comment_start = 0;
	string::size_type breakpoint = 0;
	string::size_type equal_sign_pos = 0;

	bool use_header_func = false;
	vector<string> names, values;
	while(!in.eof()){
		getline(in, buf);

		// ';'�ȍ~�̓R�����g�Ƃ��Ė���
		if( (comment_start = buf.find(';')) != string::size_type(-1) )
			buf = buf.substr(0, comment_start);

		// '#'�ȍ~�̓R�����g�Ƃ��Ė���
		if( (comment_start = buf.find('#')) != string::size_type(-1) )
			buf = buf.substr(0, comment_start);

		// '//'�ȍ~�̓R�����g�Ƃ��Ė���
		if( (comment_start = buf.find("//")) != string::size_type(-1) )
			buf = buf.substr(0, comment_start);

		while((breakpoint = buf.find("\t")) != string::size_type(-1)){
			buf.erase(breakpoint, 1);
		}
		while((breakpoint = buf.find(" ")) != string::size_type(-1)){
			buf.erase(breakpoint, 1);
		}

		if(buf.empty())
			continue;

		// �w�b�_�s�̃`�F�b�N
		if( buf.at(0) == '[' ){
			if(use_header_func && !names.empty()){
				m_mapHeaderFunc[cur_header](&names[0], &values[0], (int)names.size(), cur_header);
			}

			if( (header_end = buf.find(']')) == string::size_type(-1) ){
				cout << "[rxINI::Load] Header tag opened with '[' but not closed with ']'" << endl;
				return 0;
			}

			cur_header = buf.substr(1, header_end-1);

			if(!m_mapHeaderFunc.empty()){
				map<string, RX_HEADER_FUNC>::iterator i = m_mapHeaderFunc.find(cur_header);
				if(i == m_mapHeaderFunc.end()){
					use_header_func = false;
				}
				else{
					use_header_func = true;
					names.clear();
					values.clear();
				}
			}
		
			continue;
		}

		// ���ڍs
		if( (equal_sign_pos = buf.find('=')) == string::size_type(-1) ){
			cout << "[rxINI::Load] Value '" << buf;
			cout << "' is not a header and not a valid value, missing '='." << endl;
			return 0;
		}

		name = buf.substr(0, equal_sign_pos);
		value = buf.substr(equal_sign_pos + 1);

		if(use_header_func){
			names.push_back(name);
			values.push_back(value);
		}

		bool preset = false;
		RX_INIFOR(i, m_vContainer){
			if(i->Name == name){
				i->Value = value;
				if(i->Var != NULL){
					if(i->Type == typeid(float).name()){
						*((float*)i->Var) = boost::lexical_cast<float>(value);
						cout << "[setting] " << name << " : " << value;
						cout << " (float : " << *((float*)i->Var) << ")" << endl;
					}
					else if(i->Type == typeid(double).name()){
						*((double*)i->Var) = boost::lexical_cast<double>(value);
						cout << "[setting] " << name << " : " << value;
						cout << " (double : " << *((double*)i->Var) << ")" << endl;
					}
					else if(i->Type == typeid(double*).name()){
						//*((double**)i->Var) = boost::lexical_cast<double>(value);
						cout << "[setting] " << name << " : " << value;
						cout << " (double* : " << (*((double**)i->Var))[0] << ")" << endl;
					}
					else if(i->Type == typeid(long double).name()){
						*((long double*)i->Var) = boost::lexical_cast<long double>(value);
						cout << "[setting] " << name << " : " << value;
						cout << " (long double : " << *((long double*)i->Var) << ")" << endl;
					}
					else if(i->Type == typeid(int).name()){
						*((int*)i->Var) = boost::lexical_cast<int>(value);
						cout << "[setting] " << name << " : " << value;
						cout << " (int : " << *((int*)i->Var) << ")" << endl;
					}
					else if(i->Type == typeid(unsigned int).name()){
						*((unsigned int*)i->Var) = boost::lexical_cast<unsigned int>(value);
						cout << "[setting] " << name << " : " << value;
						cout << " (unsigned int : " << *((unsigned int*)i->Var) << ")" << endl;
					}
					else if(i->Type == typeid(short).name()){
						*((short*)i->Var) = boost::lexical_cast<short>(value);
						cout << "[setting] " << name << " : " << value;
						cout << " (short : " << *((short*)i->Var) << ")" << endl;
					}
					else if(i->Type == typeid(unsigned short).name()){
						*((unsigned short*)i->Var) = boost::lexical_cast<unsigned short>(value);
						cout << "[setting] " << name << " : " << value;
						cout << " (unsigned short : " << *((unsigned short*)i->Var) << ")" << endl;
					}
					else if(i->Type == typeid(long).name()){
						*((long*)i->Var) = boost::lexical_cast<long>(value);
						cout << "[setting] " << name << " : " << value;
						cout << " (long : " << *((long*)i->Var) << ")" << endl;
					}
					else if(i->Type == typeid(unsigned long).name()){
						*((unsigned long*)i->Var) = boost::lexical_cast<unsigned long>(value);
						cout << "[setting] " << name << " : " << value;
						cout << " (unsigned long : " << *((unsigned long*)i->Var) << ")" << endl;
					}
					else if(i->Type == typeid(string).name()){
						*((string*)i->Var) = boost::lexical_cast<string>(value);
						cout << "[setting] " << name << " : " << value;
						cout << " (string : " << *((string*)i->Var) << ")" << endl;
					}
					else if(i->Type == typeid(char).name()){
						*((char*)i->Var) = boost::lexical_cast<char>(value);
						cout << "[setting] " << name << " : " << value;
						cout << " (char : " << *((char*)i->Var) << ")" << endl;
					}
					else if(i->Type == typeid(unsigned char).name()){
						*((unsigned char*)i->Var) = boost::lexical_cast<unsigned char>(value);
						cout << "[setting] " << name << " : " << value;
						cout << " (unsigned char : " << *((unsigned char*)i->Var) << ")" << endl;
					}
					else if(i->Type == typeid(bool).name()){
						*((bool*)i->Var) = boost::lexical_cast<bool>(value);
						cout << "[setting] " << name << " : " << value;
						cout << " (bool : " << *((bool*)i->Var) << ")" << endl;
					}
				}
				preset = true;
				break;
			}
		}

		if(!preset) m_vContainer.push_back(CreatePair(cur_header, name, value));
	}

	return 1;
}

/*!
 * �ݒ�t�@�C�����ڂ�string value���X�V
 */
inline int rxINI::UpdateValues(void)
{
	RX_INIFOR(i, m_vContainer){
		if(i->Var != NULL){
			if(i->Type == typeid(float).name()){
				i->Value = boost::lexical_cast<string>(*((float*)i->Var));
			}
			else if(i->Type == typeid(double).name()){
				i->Value = boost::lexical_cast<string>(*((double*)i->Var));
			}
			else if(i->Type == typeid(long double).name()){
				i->Value = boost::lexical_cast<string>(*((long double*)i->Var));
			}
			else if(i->Type == typeid(int).name()){
				i->Value = boost::lexical_cast<string>(*((int*)i->Var));
			}
			else if(i->Type == typeid(unsigned int).name()){
				i->Value = boost::lexical_cast<string>(*((unsigned int*)i->Var));
			}
			else if(i->Type == typeid(short).name()){
				i->Value = boost::lexical_cast<string>(*((short*)i->Var));
			}
			else if(i->Type == typeid(unsigned short).name()){
				i->Value = boost::lexical_cast<string>(*((unsigned short*)i->Var));
			}
			else if(i->Type == typeid(long).name()){
				i->Value = boost::lexical_cast<string>(*((long*)i->Var));
			}
			else if(i->Type == typeid(unsigned long).name()){
				i->Value = boost::lexical_cast<string>(*((unsigned long*)i->Var));
			}
			else if(i->Type == typeid(string).name()){
				i->Value = boost::lexical_cast<string>(*((string*)i->Var));
			}
			else if(i->Type == typeid(char).name()){
				i->Value = boost::lexical_cast<string>(*((char*)i->Var));
			}
			else if(i->Type == typeid(unsigned char).name()){
				i->Value = boost::lexical_cast<string>(*((unsigned char*)i->Var));
			}
			else if(i->Type == typeid(bool).name()){
				i->Value = boost::lexical_cast<string>(*((bool*)i->Var));
			}
		}
	}

	return 1;
}

/*!
 * �ݒ�ۑ�
 * @param[in] path  �ݒ�t�@�C���p�X(""�Ȃ���͂��g�p)
 * @return -1�ŃG���[
 */
inline int rxINI::Save(const string& path)
{
	return SaveList(&m_vContainer, path);
}

/*!
 * ���ڃ��X�g���w�肵�Đݒ�ۑ�
 * @param[in] pairs ���ڃ��X�g
 * @param[in] path  �ݒ�t�@�C���p�X(""�Ȃ���͂��g�p)
 * @return -1�ŃG���[
 */
inline int rxINI::SaveList(vector<rxINIPair> *pairs, const string& path)
{
	if(!pairs){
		cout << "[rxINI::Save] Null-pointer specified for 'pairs' argument." << endl;
		return 0;
	}

	string opath = path;
	if(path == "") opath = m_strPath;

	cout << "saving the settings to " << opath << "..." << endl;

	ofstream out(opath.c_str(), ios::out);
	if(!out || !out.is_open() || out.bad() || out.fail()){
		cout << "[rxINI::Save] Cannot save file at path " << opath << endl;
		return 0;
	}

	string current_header = "";

	UpdateValues();
	for(vector<rxINIPair>::iterator i = pairs->begin(); i < pairs->end(); ++i){
		if(i->Header != current_header){
			out << '[' << i->Header << ']' << '\n';
			current_header = i->Header;
		}

		// Write value
		out << i->Name << '=' << i->Value << '\n';
	}

	return 1;
}

/*!
 * ���ڃ��X�g���w�肵�Đݒ�ۑ�
 * @param[in] pairs ���ڃ��X�g
 * @param[in] size  ���ڃ��X�g�̃T�C�Y
 * @param[in] path  �ݒ�t�@�C���p�X(""�Ȃ���͂��g�p)
 * @return -1�ŃG���[
 */
inline int rxINI::SaveList(rxINIPair *pairs, unsigned int size, const string& path)
{
	vector<rxINIPair> vpairs(pairs, pairs+size);
	return SaveList(&vpairs, path);
}

#endif //_RX_ATOM_INI_H_