/* 
 * File:   Writable.h
 * Author: ronaldo
 *
 * Created on 30 de agosto de 2023, 18:07
 */

#ifndef WRITABLE_H
#define WRITABLE_H

#include <ostream>

class Writable {
public:
    
    virtual void writeHeader(std::ostream& fs) const = 0;    
    virtual void writeRow(std::ostream& fs) const = 0;
    
    virtual std::string separator() const{
        return ";";
    }
    
    static void uintWrite(const Writable& writable, std::ostream& fs, unsigned value, bool separator = false, bool endline = false){
        fs << value;
        if(separator) sepWrite(writable, fs);
        if(endline) endlWrite(writable, fs);
    }
    
    static void intWrite(const Writable& writable, std::ostream& fs, int value, bool separator = false, bool endline = false){
        fs << value;
        if(separator) sepWrite(writable, fs);
        if(endline) endlWrite(writable, fs);        
    }
    
    static void floatWrite(const Writable& writable, std::ostream& fs, float value, bool separator = false, bool endline = false){
        fs << value;
        if(separator) sepWrite(writable, fs);
        if(endline) endlWrite(writable, fs);        
    }
    
    static void strWrite(const Writable& writable, std::ostream& fs, std::string value, bool separator = false, bool endline = false){
        fs << value;
        if(separator) sepWrite(writable, fs);
        if(endline) endlWrite(writable, fs);       
    }
    
    static void sepWrite(const Writable& writable, std::ostream& fs){        
        fs << writable.separator();                
    }
    
    static void endlWrite(const Writable& writable, std::ostream& fs){        
        fs << std::endl;                
    }
    
};

#endif /* WRITABLE_H */

