#include "_token.h"
#include "_agent.h"
#include "_fractioningRateAlgorithm.h"


void _token::listAgents(const std::function<bool(_agent&)> function)const{
    
    for (auto pagent : agents) {
        
        if(function(*pagent.second)) return;

    }

}

void _token::draw(const Render& render) const{
    
    for (auto pagent : agents) {
        
        pagent.second->draw(render);

    }
    
}

bool _token::selectNewRestEndpointToAgent(const _agent& agent, _site& selectNewSite) const {

    unsigned min_distance = 0xffffffff;
    
    for (auto endpoint : endpoints) {
        
        bool flag = true;

        this->listPendingTasks([endpoint, &flag](const _task & task) {

            if (task.getDelivery().match(endpoint)) {

                flag = false;
                
                return true;                

            }

            return false;

        });
        
        if(flag){
        
            this->listAgents([endpoint, agent, &flag](_agent & otherAgent) {

                if (agent.id() != otherAgent.id() && otherAgent.goalSite().match(endpoint)) { //other agents

                    flag = false;
                    
                    return true;

                } 

                return false;

            });
                
        }
        
        if(flag){
            
            unsigned distance = this->endpointsDistanceAlgorithm.solve(agent.currentSite(), endpoint);
            
            if (distance < min_distance) {
                min_distance = distance;
                selectNewSite = endpoint;
            }
            
            
        }       

    }

    return min_distance < 0xffffffff;

}

bool _token::selectNewRestEndpointPathToAgent(const _agent& agent, _task& conflictTask, _stepPath& restPath) const {
    
    _stepAstarAlgorithm astar;
    _site endpoint;
    
    if(isConflictingSiteWithAnyTaskDelivery(agent.currentSite(), conflictTask)){
        
        if(selectNewRestEndpointToAgent(agent, endpoint)){

            if(astar.solve(stepMap, restPath, endpoint, agent.id())){

                return true;

            } else {

                try {
                    std::ostringstream stream;
                    stream << "unsolved rest endpoint path";
                    MAPD_EXCEPTION(stream.str());
                } catch (std::exception& e) {
                    std::cout << e.what() << std::endl;
                    std::abort();
                }

            }

        } else {

            try {
                std::ostringstream stream;
                stream << "unsolved new rest endpoint selection";
                MAPD_EXCEPTION(stream.str());
            } catch (std::exception& e) {
                std::cout << e.what() << std::endl;
                std::abort();
            }

        }
    
    }
    
    return false;


}

bool _token::isConflictingSiteWithAnyTaskDelivery(const _site& site, _task& conflitTask) const {

    bool conflit = false;

    this->listPendingTasks([site, &conflit, &conflitTask, this](const _task & task) {

        if (task.getDelivery().match(site)) {

            conflit = true;
            
            conflitTask = task;

            return true;

        }

        return false;

    });

    return conflit;

}

bool _token::selectNewTaskToAgent(const _agent& agent, _task& selectedTask) const{

    unsigned min_distance = 0xffffffff;
        
    this->listPendingTasks([&min_distance, agent, &selectedTask, this](const _task& task) {
        
        bool flag = true;
        
        this->listAgents([task, &flag, agent, this](const _agent & otherAgent) {
            
            if (otherAgent.id() != agent.id()) {
                
                if (otherAgent.goalSite().match(task.getPickup()) || otherAgent.goalSite().match(task.getDelivery())) {
                    
                    flag = false;
                    
                    return true;
                    
                }
                
            }
            
            return false;
            
        });
        
        if(flag){
            
            unsigned distance = this->endpointsDistanceAlgorithm.solve(agent.currentSite(), task.getPickup());

            if (distance < min_distance) {
                min_distance = distance;
                selectedTask = task;
            }
            
        }
        
        return false;

    });

    return min_distance < 0xffffffff;

}

bool _token::taskPathToAgentToAgent(const _agent& agent, const _task& task, _stepPath& path) const{
    
    _stepAstarAlgorithm astar;

    bool flag = astar.solve(stepMap, path, task.getPickup(), agent.id());

    if (flag) {
        
        flag = astar.solve(stepMap, path, task.getDelivery(), agent.id());

        if (flag) {
            
            return true;

        } else {

            try {
                std::ostringstream stream;
                stream << "unsolved task delivery endpoint path: " << task;
                MAPD_EXCEPTION(stream.str());
            } catch (std::exception& e) {
                std::cout << e.what() << std::endl;
                std::abort();
            }

        }

    } else {

        try {
            std::ostringstream stream;
            stream << "unsolved task pickup endpoint path: " << task;
            MAPD_EXCEPTION(stream.str());
        } catch (std::exception& e) {
            std::cout << e.what() << std::endl;
            std::abort();
        }

    }
    
    return false;

}

bool _token::selectNewTaskPathToAgent(const _agent& agent, _task& selectedTask, _stepPath& path) const{
    
    bool flag = selectNewTaskToAgent(agent, selectedTask);
    
    if(flag){
        
        return taskPathToAgentToAgent(agent, selectedTask, path);
        
    }
    
    return false;
    
}

bool _token::updateTaskPathToAgent(_agent& agent){
    
    _task newTask;
    _stepPath path(agent.currentSite());
    bool flag = selectNewTaskPathToAgent(agent, newTask, path);
    
    if(flag){
        
        assignTask(newTask, agent);
        reportTaskUpdate(newTask.id(), agent.id(), ReportTask::PathType::task, path);
        
        agent.designTask(newTask);
        agent.setPathMoving(path, stepMap);
        
        if (agent.isParked()) {
                
            runTask((newTask));
            finishTask(newTask);

            agent.undesignTask();
            agent.setTrivialPathMoving(stepMap);   

        } 

        return true;
        
    }
    
    
    return false;
    
}

bool _token::updateRestPathToAgent(_agent& agent){
    
    _task conflict;
    
    _stepPath restPath(agent.currentSite());
    
    if(selectNewRestEndpointPathToAgent(agent, conflict, restPath)){
        
        reportTaskUpdate(conflict.id(), agent.id(), ReportTask::PathType::rest, restPath);  

        agent.setPathMoving(restPath, stepMap);
        
        return true;
        
    }
    
    return false;
    
}

void _token::updatePath(_agent& agent){
    
    if(agent.isParked()){
    
        if(!updateTaskPathToAgent(agent))
            if(!updateRestPathToAgent(agent))
                this->updateTrivialPathToAgent(agent);
    
    } else {
        
        try {
            std::ostringstream stream;
            stream << "agent not parked: " << std::endl <<agent;
            MAPD_EXCEPTION(stream.str());
        } catch (std::exception& e) {
            std::cout << e.what() << std::endl;
            std::abort();
        }
        
    }
    
}


void _token::updateTrivialPathToAgent(_agent& agent){
        
    _stepSite site = agent.goalSite();
    _stepPath path(site);
    site.SetStep(site.GetStep() + 1);
    path.progress(site);

    agent.setPathMoving(path, stepMap);      

}


// versão 1: partindo de delivery original, escolhe o primeiro task endpoint que 
// atende à razão de fracionamento. 
// Se nenhum endpoint atender, retorna falso.
// Nesse caso, a tarefa não será selecionada.
bool _token::selectNewTaskPathToAgent(const _agent& agent, _task& origTask, _c_task& c_task, _stepPath& path, _stepPath& c_path, bool& c_taskFlag, float threshold) const{
    
    _fractioningRateAlgorithm fractioningRateAlgorithm(endpointsDistanceAlgorithm);
    
    bool flag = selectNewTaskPathToAgent(agent, origTask, path);
    
    if(flag){
        
        if(fractioningRateAlgorithm.solve(path, threshold)){
            
            return flag;
            
        } else {
            
            flag = false;
        
            int size = path.size();
            path.list([&flag, fractioningRateAlgorithm, &size, agent, threshold, origTask, &c_task, path,  &c_path, &c_taskFlag, this](const _stepSite& site){// voltando

                if(!site.match(origTask.getPickup())){ // até pickup

                    if(this->map.getType(site.GetRow(), site.GetColunm()) == _map::Type::task){ // encontra um task endpoint

                        bool flag3 = true;
                        this->listAgents([&flag3, site, agent, this](const _agent & otherAgent) { // verifica se o task endpoint está disponível

                            if (otherAgent.id() != agent.id()) {

                                if (otherAgent.goalSite().match(site)) {

                                    flag3 = false;

                                    return true;

                                }

                            }

                            return false;

                        });

                        if(flag3){ // task endpoint válido (verificação ok)
                            
                            if(fractioningRateAlgorithm.solve(path.currentSite(), site, size, threshold)){ // encontra um novo endpoint que atende ao threshold
                                
                                c_task.setPickup(origTask.getPickup());
                                c_task.setDelivery(site);
                                c_task.SetConcernTaskId(origTask.id());
                                                                
                                c_taskFlag = this->taskPathToAgentToAgent(agent, c_task, c_path);
                                
                                if(c_taskFlag){
                                    
                                    flag = true;
                                    
                                    return true;
                                    
                                } else {
                                    
                                    try {
                                        std::ostringstream stream;
                                        stream << "unsolved c_task path: " << c_task;
                                        MAPD_EXCEPTION(stream.str());
                                    } catch (std::exception& e) {
                                        std::cout << e.what() << std::endl;
                                        std::abort();
                                    }                              
                                    
                                }
                                
                            }

                        }

                    }

                } else {

                    return true;

                }  
                
                size--;

                return false;

            });
        
        }
                        
    }
    
    return flag;
    
}