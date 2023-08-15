#include "_token.h"
#include "_agent.h"

_token::_token(
        const SiteMap& siteMap, 
        const IntegerMap& integerMap, 
        const std::vector<_agent>& agents,
        const std::vector<_site>& endpoints) : 
            siteMap(siteMap), 
            integerMap(integerMap),
            agents(agents),
            endpoints(endpoints){}

void _token::listAgents(const std::function<bool(_agent&)> function){
   
    std::vector<_agent>::iterator it = agents.begin();
    for(;it != agents.end();it++){
        if(function(*it))return;
    }

}

void _token::draw(const Render& render) const{
        
    std::vector<_agent>::const_iterator it = agents.begin();
    for(;it != agents.end();it++) it->draw(render);

}