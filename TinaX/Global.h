#pragma once

class Global
{
public:
    static Global& instance(){ static Global g; return g; }

public:

private:

private:
    Global();
    virtual ~Global();
    bool operator = ( const Global& ){}
    Global( const Global& ){}
};

