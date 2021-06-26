 #include"response.hpp"
Response::Response(){}

const String            &Response::getVersProtocol() const
{
    return this->_versProtocol;
}

const String            &Response::getNumError() const
{
     return this->_NumError;
}

const String            &Response::getDescriptionError() const
{
     return this->_descriptionError;
}

const String            &Response::getContentType() const
{
     return this->_contentType;
}

const String            &Response::getFileLength() const
{
     return this->_FileLength;
}

const int               &Response::getINumError() const
{
     return this->_iNumError;
}

const unsigned long    &Response:: getIFileLength() const
{   
     return this->_iFileLength;
}

//=============================================================================
// Функция принимает на вход имя файла (fn), находит его расширение, 
// проверяет есть ли данное расширение в заданном заранее Map контейнере, 
// если ключ удалось найти, устанавливает в MIME type (this->_contentType)
// значение найденное по ключу. 
//=============================================================================


String Response::findType(std::string fn)
{   
    std::string fn1 = fn.substr(fn.find_last_of(".") + 1);
    if(this->_types.count(fn1) == 1)
        this->_contentType = this->_types[fn1];
    return( this->_contentType);
}

//=============================================================================
//
//
//
//
//
//=============================================================================


String                  Response::descriptionError(std::string numError)
{
   
  if(numError == "200")  this->_descriptionError = "OK";
  if(numError == "404")  this->_descriptionError = "Not Found";
  if(numError == "400")  this->_descriptionError = "Bad request";
    return(this->_descriptionError);
}

//=============================================================================
//
//
//
//
//
//=============================================================================


int                  Response::findFile(std::string NewPath)
{
    std::ifstream file(NewPath);  
    if (!file.is_open()) 
    {
        return(ERROR);
    }
    std::string   str((std::istreambuf_iterator<char>(file)),
                    std::istreambuf_iterator<char>());
    
    this->_File  = str;
    this->_FileLength = std::to_string(str.size());
    this->_contentType = findType(NewPath);
    return(SUCCESS);
}

//=============================================================================
//
//
//
//
//
//=============================================================================


String                  Response::creatRespons(ParseRequest &Request, std::string numError)
{
    int check = 0;
    this->_errorFilePath = Request.getErrorFilePath();
    this->_types = Request.getType();
    //this->_FileLength = std::to_string(Request.getSizeFile());
    this->_NumError = numError;
    this->_contentType = Request.getContentType();
    this->_FileLength = std::to_string(Request.getSizeFile());
    if(this->_NumError != "200")
        check = findFile(this->_errorFilePath);//?
    else
        this->_File = Request.getStr();
    
    this->_versProtocol = Request.getVersProtocol();
    this->_descriptionError = descriptionError(this->_NumError);
    
    
    std::string result ;
    if (check == ERROR)
        result = this->_versProtocol + " " + this->_NumError + " " + this->_descriptionError + "\r\n";
    else if (this->_NumError == "200")
        result = this->_versProtocol + " " + this->_NumError + " " + this->_descriptionError + "\r\n" + "Content-Length: " + this->_FileLength + "\r\n" 
        + "Content-Type: " + this->_contentType + "\r\n\r\n" + this->_File + "\r\n\r\n";
    else
        result = "HTTP/1.1 "+ this->_NumError + " " + this->_descriptionError + "\r\n" + "Content-Length: " + this->_FileLength + "\r\n" 
        + "Content-Type: " + this->_contentType + "\r\n\r\n" + this->_File + "\r\n\r\n";
    return(result);

}

