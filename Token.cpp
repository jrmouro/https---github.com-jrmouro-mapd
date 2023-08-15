#include "Token.h"
#include "Agent.h"

Token::Token(
        const SiteMap& siteMap, 
        const IntegerMap& integerMap, 
        const std::vector<Agent>& agents,
        const std::vector<_site>& endpoints) : 
            siteMap(siteMap), 
            integerMap(integerMap),
            agents(agents),
            endpoints(endpoints){}

void Token::listAgents(const std::function<bool(Agent&)> function){
   
    std::vector<Agent>::iterator it = agents.begin();
    for(;it != agents.end();it++){
        if(function(*it))return;
    }

}

void Token::draw(const Render& render) const{
        
    std::vector<Agent>::const_iterator it = agents.begin();
    for(;it != agents.end();it++) it->draw(render);

}