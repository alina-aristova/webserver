 #include"response.hpp"
 Response::Response()
 {

 }
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
String Response::findType(std::string fn)
{   
    
    //std::cout << "WTW:"  << fn << "\n";
    std::string fn1 = fn.substr(fn.find_last_of(".") + 1);
    //std::cout << "тип файла:"  << fn1 << "\n";
    if(this->_types.count(fn1) == 1)
        this->_contentType = this->_types[fn1];
    //std::cout <<this->_contentType<< ":this->_contentType" << std::endl;
    return( this->_contentType);

}
String                  Response::descriptionError(std::string numError)
{
   
  if(numError == "200")  this->_descriptionError = "OK";
  if(numError == "404")  this->_descriptionError = "Not Found";
  if(numError == "400")  this->_descriptionError = "Bad request";
    return(this->_descriptionError);
}

String                  Response::findFile(std::string NewPath )
{
      //std::cout << "File: "<< File << std::endl;
  
    
    {        std::ifstream file(NewPath);  
        if (!file.is_open()) 
        {
           ;
        }
        std::string   str((std::istreambuf_iterator<char>(file)),
                        std::istreambuf_iterator<char>());
        
        this->_File  = str;//найти длину
        this->_FileLength = std::to_string(str.size());
        this->_contentType = findType(NewPath);
        return(this->_File);}
}

// String                  Response::getResponse(ParseRequest &Request,std::string Method, std::string numError)
// {

   
// }

String                  Response::creatRespons(ParseRequest &Request, std::string numError)
{
    
    this->_FileLength = std::to_string(Request.getSizeFile());
    this->_NumError = numError;
    this->_contentType = Request.getContentType();
    this->_FileLength = std::to_string(Request.getSizeFile());
    if(this->_NumError != "200")
    
        this->_File = findFile( "/Users/acase/Desktop/Error/error.pdf");
    else
        this->_File = Request.getStr();
    
    this->_versProtocol = Request.getVersProtocol();
    this->_descriptionError = descriptionError(this->_NumError);
    
   
    std::string result ;
    if (this->_NumError == "200")
        result = this->_versProtocol + " " + this->_NumError + " " + this->_descriptionError + "\r\n" + "Content-Length: " + this->_FileLength + "\r\n" 
        + "Content-Type: " + this->_contentType + "\r\n\r\n" + this->_File + "\r\n\r\n";
    else
        result = "HTTP/1.1 "+ this->_NumError + " " + this->_descriptionError + "\r\n" + "Content-Length: " + this->_FileLength + "\r\n" 
        + "Content-Type: " + this->_contentType + "\r\n\r\n" + this->_File + "\r\n\r\n";
    return(result);

}

void Response::addTypes()
{
    this->_types.insert( std::make_pair("aac","audio/aac"));
    this->_types.insert( std::make_pair("abw","application/x-abiword"));
    this->_types.insert( std::make_pair("arc","application/x-freearc"));
    this->_types.insert( std::make_pair("avi","video/x-msvideo"));
    this->_types.insert( std::make_pair("azw","application/vnd.amazon.ebook"));
    this->_types.insert( std::make_pair("bin","application/octet-stream"));
    this->_types.insert( std::make_pair("bmp","image/bmp"));
    this->_types.insert( std::make_pair("bz","application/x-bzip"));
    this->_types.insert( std::make_pair("bz2","application/x-bzip2"));
    this->_types.insert( std::make_pair("csh","application/x-csh"));
    this->_types.insert( std::make_pair("css","text/css"));
    this->_types.insert( std::make_pair("csv","text/csv"));
    this->_types.insert( std::make_pair("doc","application/msword"));
    this->_types.insert( std::make_pair("docx","application/vnd.openxmlformats-officedocument.wordprocessingml.document"));
    this->_types.insert( std::make_pair("eot","application/vnd.ms-fontobject"));
    this->_types.insert( std::make_pair("epub","application/epub+zip"));
    this->_types.insert( std::make_pair("gz","application/gzip"));
    this->_types.insert( std::make_pair("gif","image/gif"));
    this->_types.insert( std::make_pair("htm","text/html"));
    this->_types.insert( std::make_pair("html","text/html"));
    this->_types.insert( std::make_pair("ico","image/vnd.microsoft.icon"));
    this->_types.insert( std::make_pair("ics","text/calendar"));
    this->_types.insert( std::make_pair("jar","application/java-archive"));
    this->_types.insert( std::make_pair("jpeg","image/jpeg"));
    this->_types.insert( std::make_pair("jpg","image/jpeg"));
    this->_types.insert( std::make_pair("js","text/javascript"));
    this->_types.insert( std::make_pair("json","application/json"));
    this->_types.insert( std::make_pair("jsonld","application/ld+json"));
    this->_types.insert( std::make_pair("mid","audio/midi audio/x-midi"));
    this->_types.insert( std::make_pair("midi","audio/midi audio/x-midi"));
    this->_types.insert( std::make_pair("mjs","text/javascript"));
    this->_types.insert( std::make_pair("mp3","audio/mpeg"));
    this->_types.insert( std::make_pair("mpeg","video/mpeg"));
    this->_types.insert( std::make_pair("mpkg","application/vnd.apple.installer+xml"));
    this->_types.insert( std::make_pair("odp","application/vnd.oasis.opendocument.presentation"));
    this->_types.insert( std::make_pair("ods","application/vnd.oasis.opendocument.spreadsheet"));
    this->_types.insert( std::make_pair("odt","application/vnd.oasis.opendocument.text"));
    this->_types.insert( std::make_pair("oga","audio/ogg"));
    this->_types.insert( std::make_pair("ogv","video/ogg"));
    this->_types.insert( std::make_pair("ogx","application/ogg"));
    this->_types.insert( std::make_pair("opus","audio/opus"));
    this->_types.insert( std::make_pair("otf","font/otf"));
    this->_types.insert( std::make_pair("png","image/png"));
    this->_types.insert( std::make_pair("pdf","application/pdf"));
    this->_types.insert( std::make_pair("php","application/x-httpd-php"));
    this->_types.insert( std::make_pair("ppt","application/vnd.ms-powerpoint"));
    this->_types.insert( std::make_pair("pptx","application/vnd.openxmlformats-officedocument.presentationml.presentation"));
    this->_types.insert( std::make_pair("rar","application/vnd.rar"));
    this->_types.insert( std::make_pair("rtf","application/rtf"));
    this->_types.insert( std::make_pair("sh","application/x-sh"));
    this->_types.insert( std::make_pair("svg","image/svg+xml"));
    this->_types.insert( std::make_pair("swf","application/x-shockwave-flash"));
    this->_types.insert( std::make_pair("tar","application/x-tar"));
    this->_types.insert( std::make_pair("tif","image/tiff"));
    this->_types.insert( std::make_pair("tiff","image/tiff"));
    this->_types.insert( std::make_pair("ts","video/mp2t"));
    this->_types.insert( std::make_pair("ttf","font/ttf"));
    this->_types.insert( std::make_pair("txt","text/plain"));
    this->_types.insert( std::make_pair("vsd","application/vnd.visio"));
    this->_types.insert( std::make_pair("wav","audio/wav"));
    this->_types.insert( std::make_pair("weba","audio/webm"));
    this->_types.insert( std::make_pair("webm","video/webm"));
    this->_types.insert( std::make_pair("webp","image/webp"));
    this->_types.insert( std::make_pair("woff","font/woff"));
    this->_types.insert( std::make_pair("woff2","font/woff2"));
    this->_types.insert( std::make_pair("xhtml","application/xhtml+xml"));
    this->_types.insert( std::make_pair("xls","application/vnd.ms-excel"));
    this->_types.insert( std::make_pair("xlsx","application/vnd.openxmlformats-officedocument.spreadsheetml.sheet"));
    this->_types.insert( std::make_pair("xul","application/vnd.mozilla.xul+xml"));
    this->_types.insert( std::make_pair("zip","application/zip"));
    this->_types.insert( std::make_pair("7z","application/x-7z-compressed"));


}