#include "_token.h"
#include "_agent.h"
#include "_thresholdAlgorithm.h"

int _token::energy()const{
    int ret = 0;
    listAgents([&ret](_agent& agent){
        ret += agent.amountEnergy();
        return false;
    });
    return ret;
}

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

bool _token::selectChargingEndpointToAgent(const _agent& agent, _site& selectNewSite) const {

    unsigned min_distance = 0xffffffff;
    
    for (auto endpoint : chargingEndpoints) {
        
        bool flag = true;

        this->listPendingTasks([endpoint, &flag](const _task & task) { // TODO isso nunca deve acontecer... testar e retirar

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

bool _token::selectChargingEndpointPathToAgent(const _agent& agent, _stepPath& chargingPath) const {
    
    _stepAstarAlgorithm astar;
    _site endpoint;
    
    if(selectChargingEndpointToAgent(agent, endpoint)){

        if(astar.solve(stepMap, chargingPath, endpoint, agent.id())){

            return true;

        } else {

            try {
                std::ostringstream stream;
                stream << "unsolved charging endpoint path";
                MAPD_EXCEPTION(stream.str());
            } catch (std::exception& e) {
                std::cout << e.what() << std::endl;
                std::abort();
            }

        }

    } else {

        try {
            std::ostringstream stream;
            stream << "unsolved charging endpoint selection";
            MAPD_EXCEPTION(stream.str());
        } catch (std::exception& e) {
            std::cout << e.what() << std::endl;
            std::abort();
        }

    }
    
    return false;

}

bool _token::selectChargingEndpointPathToAgent(const _agent& agent, _task& conflictTask, _stepPath& chargingPath) const {
    
    _stepAstarAlgorithm astar;
    _site endpoint;
    
    if(isConflictingSiteWithAnyTaskDelivery(agent.currentSite(), conflictTask)){
        
        if(selectChargingEndpointToAgent(agent, endpoint)){

            if(astar.solve(stepMap, chargingPath, endpoint, agent.id())){

                return true;

            } else {

                try {
                    std::ostringstream stream;
                    stream << "unsolved charging endpoint path";
                    MAPD_EXCEPTION(stream.str());
                } catch (std::exception& e) {
                    std::cout << e.what() << std::endl;
                    std::abort();
                }

            }

        } else {

            try {
                std::ostringstream stream;
                stream << "unsolved charging endpoint selection";
                MAPD_EXCEPTION(stream.str());
            } catch (std::exception& e) {
                std::cout << e.what() << std::endl;
                std::abort();
            }

        }
    
    }
    
    return false;

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

// seleciona tarefas somente que atendam ao threshold da tarefa e do carregamento
bool _token::selectNewTaskPathToAgentTaskCarryThreshold(const _agent& agent, _task& selectedTask, _stepPath& selectedPath) const{
    
    _thresholdAlgorithm thresholdAlgorithm(endpointsDistanceAlgorithm);

    unsigned min_distance = 0xffffffff;
        
    this->listPendingTasks([&min_distance, thresholdAlgorithm, agent, &selectedTask, &selectedPath, this](const _task& task) {
        
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
            
            unsigned pickupStep;
            _stepPath path(agent.currentSite());
            
            flag = taskPathToAgent(agent, task, path, pickupStep);
            
            if(flag){
                
                if(thresholdAlgorithm.solve(path, pickupStep, task_threshold)){ // threshold da tarefa
                
                    pickupStep = path.goalSite().GetStep() - pickupStep;

                    if(thresholdAlgorithm.solve(task.getPickup(), task.getDelivery(), pickupStep, task_threshold)){ // threshold do carregamento

                        unsigned distance = this->endpointsDistanceAlgorithm.solve(agent.currentSite(), task.getPickup());

                        if (distance < min_distance) {
                            min_distance = distance;
                            selectedTask = task;
                            selectedPath = path;
                        }

                    }
                
                }
                
            } else {
                
                try {
                    std::ostringstream stream;
                    stream << "unsolved task path: " << task;
                    MAPD_EXCEPTION(stream.str());
                } catch (std::exception& e) {
                    std::cout << e.what() << std::endl;
                    std::abort();
                }               
                
            }
            
        }
        
        return false;

    });

    return min_distance < 0xffffffff;

}

// seleciona tarefas somente que atendam ao threshold do carregamento
bool _token::selectNewTaskPathToAgentCarryThreshold(const _agent& agent, _task& selectedTask, _stepPath& selectedPath) const{
    
    _thresholdAlgorithm thresholdAlgorithm(endpointsDistanceAlgorithm);

    unsigned min_distance = 0xffffffff;
        
    this->listPendingTasks([&min_distance, thresholdAlgorithm, agent, &selectedTask, &selectedPath, this](const _task& task) {
        
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
            
            unsigned pickupStep;
            _stepPath path(agent.currentSite());
            
            flag = taskPathToAgent(agent, task, path, pickupStep);
            
            if(flag){
                
                pickupStep = path.goalSite().GetStep() - pickupStep;
                
                if(thresholdAlgorithm.solve(task.getPickup(), task.getDelivery(), pickupStep, task_threshold)){
            
                    unsigned distance = this->endpointsDistanceAlgorithm.solve(agent.currentSite(), task.getPickup());

                    if (distance < min_distance) {
                        min_distance = distance;
                        selectedTask = task;
                        selectedPath = path;
                    }
                
                }
                
            } else {
                
                try {
                    std::ostringstream stream;
                    stream << "unsolved task path: " << task;
                    MAPD_EXCEPTION(stream.str());
                } catch (std::exception& e) {
                    std::cout << e.what() << std::endl;
                    std::abort();
                }               
                
            }
            
        }
        
        return false;

    });

    return min_distance < 0xffffffff;

}

// seleciona tarefas somente que atendam ao threshold da tarefa
bool _token::selectNewTaskPathToAgentTaskThreshold(const _agent& agent, _task& selectedTask, _stepPath& selectedPath) const{
    
    _thresholdAlgorithm thresholdAlgorithm(endpointsDistanceAlgorithm);

    unsigned min_distance = 0xffffffff;
        
    this->listPendingTasks([&min_distance, thresholdAlgorithm, agent, &selectedTask, &selectedPath, this](const _task& task) {
        
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
            
            unsigned pickupStep;
            _stepPath path(agent.currentSite());
            
            flag = taskPathToAgent(agent, task, path, pickupStep);
            
            if(flag){
                
                if(thresholdAlgorithm.solve(path, pickupStep, task_threshold)){
            
                    unsigned distance = this->endpointsDistanceAlgorithm.solve(agent.currentSite(), task.getPickup());

                    if (distance < min_distance) {
                        min_distance = distance;
                        selectedTask = task;
                        selectedPath = path;
                    }
                
                }
                
            } else {
                
                try {
                    std::ostringstream stream;
                    stream << "unsolved task path: " << task;
                    MAPD_EXCEPTION(stream.str());
                } catch (std::exception& e) {
                    std::cout << e.what() << std::endl;
                    std::abort();
                }               
                
            }
            
        }
        
        return false;

    });

    return min_distance < 0xffffffff;

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

bool _token::chargingTaskPathToAgent(const _agent& agent, const _task& task, _stepPath& path, unsigned& pickupStep, unsigned& deliveryStep) const{
    
    _stepAstarAlgorithm astar;

    bool flag = astar.solve(stepMap, path, task.getPickup(), agent.id());

    if (flag) {
        
        pickupStep = path.goalSite().GetStep();
        
        flag = astar.solve(stepMap, path, task.getDelivery(), agent.id());

        if (flag) {
            
            deliveryStep = path.goalSite().GetStep();  
            
            _stepSite chargingSite;
            
            flag = selectChargingEndpointPathToAgent(agent, path);
            
            if(flag){
                
                return true;                             
                
            } else {
                
                try {
                    std::ostringstream stream;
                    stream << "unsolved charging endpoint path: " << task;
                    MAPD_EXCEPTION(stream.str());
                } catch (std::exception& e) {
                    std::cout << e.what() << std::endl;
                    std::abort();
                }
                
            }
            
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

bool _token::taskPathToAgent(const _agent& agent, const _task& task, _stepPath& path, unsigned& pickupStep) const{
    
    _stepAstarAlgorithm astar;

    bool flag = astar.solve(stepMap, path, task.getPickup(), agent.id());

    if (flag) {
        
        pickupStep = path.goalSite().GetStep();
        
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

bool _token::selectNewChargingTaskPathToAgent(const _agent& agent, _task& selectedTask, _stepPath& path, unsigned& pickupStep, unsigned& deliveryStep) const {
    
    bool flag = selectNewTaskToAgent(agent, selectedTask);
    
    if(flag){
        
        return chargingTaskPathToAgent(agent, selectedTask, path, pickupStep, deliveryStep);
        
    }
    
    return false;
    
}


bool _token::selectNewTaskPathToAgent(const _agent& agent, _task& selectedTask, _stepPath& path, unsigned& pickupStep) const{
    
    bool flag = selectNewTaskToAgent(agent, selectedTask);
    
    if(flag){
        
        return taskPathToAgent(agent, selectedTask, path, pickupStep);
        
    }
    
    return false;
    
}

bool _token::updateTaskPathToAgentTaskThreshold(_agent& agent, bool energyCheck){
    
    _task newTask;
    _stepPath path(agent.currentSite());
    bool flag = selectNewTaskPathToAgentTaskThreshold(agent, newTask, path);
    
    if(flag){
        
        AES aes = AES::none;
        
        if(energyCheck){
                    
            flag = agent.isAbleToFulfillTaskPath(map, newTask, path, aes);
        
        }
    
        if(flag){
        
            assignTask(newTask, agent);
            reportTaskUpdate(agent, newTask, ReportTask::PathType::task, path);

            agent.assignTask(*this, newTask, path, aes);
            stepMap.setMoving(path,agent.id());
            
            if (agent.isInFinishedPath()) {

                runTask((newTask));
                finishTask(newTask);

                agent.unassignTask(*this, aes);
                
//                flag = _stepPath::trivialSteppingPath(path);
//                
//                if(flag){
//                    
//                    agent.assignTrivialPath(*this, path, aes);   
//                    stepMap.setMoving(path,agent.id());
//                    
//                } else {
//                    
//                    try {
//                        std::ostringstream stream;
//                        stream << "invalid trivialSteppingPath: " << std::endl << agent;
//                        MAPD_EXCEPTION(stream.str());
//                    } catch (std::exception& e) {
//                        std::cout << e.what() << std::endl;
//                        std::abort();
//                    }                    
//                    
//                }
                
                return false;
                

            } 

            return true;

        }
        
    }
    
    
    return false;
    
}

bool _token::updateTaskPathToAgentCarryThreshold(_agent& agent, bool energyCheck){
    
    _task newTask;
    _stepPath path(agent.currentSite());
    bool flag = selectNewTaskPathToAgentCarryThreshold(agent, newTask, path);
    
    if(flag){
        
        AES aes = AES::none;
        
        if(energyCheck){
        
            flag = agent.isAbleToFulfillTaskPath(map, newTask, path, aes);
        
        }
    
        if(flag){

            assignTask(newTask, agent);
            reportTaskUpdate(agent, newTask, ReportTask::PathType::task, path);

            agent.assignTask(*this, newTask, path, aes);
            stepMap.setMoving(path,agent.id());

            if (agent.isInFinishedPath()) {

                runTask((newTask));
                finishTask(newTask);
                
                agent.unassignTask(*this, aes);
                
//                flag = _stepPath::trivialSteppingPath(path);
//                
//                if(flag){
//                    
//                    agent.assignTrivialPath(*this, path, aes);   
//                    stepMap.setMoving(path,agent.id());
//                    
//                } else {
//                    
//                    try {
//                        std::ostringstream stream;
//                        stream << "invalid trivialSteppingPath: " << std::endl << agent;
//                        MAPD_EXCEPTION(stream.str());
//                    } catch (std::exception& e) {
//                        std::cout << e.what() << std::endl;
//                        std::abort();
//                    }                    
//                    
//                }
                
                return false;

            } 

            return true;

        }
        
    }
    
    
    return false;
    
}

bool _token::updateTaskPathToAgentTaskCarryThreshold(_agent& agent, bool energyCheck){
    
    _task newTask;
    _stepPath path(agent.currentSite());
    bool flag = selectNewTaskPathToAgentTaskCarryThreshold(agent, newTask, path);
    
    if(flag){
        
        AES aes = AES::none;
        
        if(energyCheck){
        
            flag = agent.isAbleToFulfillTaskPath(map, newTask, path, aes);
        
        }
        
        if(flag){

            assignTask(newTask, agent);
            reportTaskUpdate(agent, newTask, ReportTask::PathType::task, path);
            
            agent.assignTask(*this, newTask, path, aes);
            stepMap.setMoving(path,agent.id());

            if (agent.isInFinishedPath()) {

                runTask((newTask));
                finishTask(newTask);

                agent.unassignTask(*this, aes);
                
//                flag = _stepPath::trivialSteppingPath(path);
//                
//                if(flag){
//                    
//                    agent.assignTrivialPath(*this, path, aes);   
//                    stepMap.setMoving(path,agent.id());
//                    
//                } else {
//                    
//                    try {
//                        std::ostringstream stream;
//                        stream << "invalid trivialSteppingPath: " << std::endl << agent;
//                        MAPD_EXCEPTION(stream.str());
//                    } catch (std::exception& e) {
//                        std::cout << e.what() << std::endl;
//                        std::abort();
//                    }                    
//                    
//                }
                
                return false;

            } 

            return true;

        }
        
    }
    
    
    return false;
    
}

bool _token::updateChargingTaskPathToAgent(_agent& agent, bool energyCheck){
    
    unsigned pickupStep, deliveryStep;
    _task newTask;
    _stepPath path(agent.currentSite());
    bool flag = selectNewChargingTaskPathToAgent(agent, newTask, path, pickupStep, deliveryStep);
    
    if(flag){
        
        AES aes = AES::none;
        
        if(energyCheck){
        
            flag = agent.isAbleToFulfillTaskPath(map, newTask, path, aes);
        
        }
    
        if(flag){

            assignTask(newTask, agent);
            reportTaskUpdate(agent, newTask, ReportTask::PathType::charging, path);

            agent.assignChargingTask(*this, newTask, path, aes);
            stepMap.setMoving(path,agent.id());
            
            if (agent.isInFinishedPath()) {

                runTask((newTask));
                finishTask(newTask);
                
                agent.unassignChargingTask(*this, aes);
                
                flag = _stepPath::trivialSteppingPath(path);
                
                if(flag){
                    
                    agent.assignChargingTrivialPath(*this, path, aes);   
                    stepMap.setMoving(path,agent.id());
                    
                } else {
                    
                    try {
                        std::ostringstream stream;
                        stream << "invalid trivialSteppingPath: " << std::endl << agent;
                        MAPD_EXCEPTION(stream.str());
                    } catch (std::exception& e) {
                        std::cout << e.what() << std::endl;
                        std::abort();
                    }                    
                    
                }
                
            } 

            return true;

        }
    
    }
        
    return false;
    
}

bool _token::updateTaskPathToAgent(_agent& agent, bool energyCheck){
    
    unsigned pickupStep;
    _task newTask;
    _stepPath path(agent.currentSite());
    bool flag = selectNewTaskPathToAgent(agent, newTask, path, pickupStep);
    
    if(flag){
        
        AES aes = AES::none;
        
        if(energyCheck){
        
            flag = agent.isAbleToFulfillTaskPath(map, newTask, path, aes);
        
        }
    
        if(flag){

            assignTask(newTask, agent);
            reportTaskUpdate(agent, newTask, ReportTask::PathType::task, path);

            agent.assignTask(*this, newTask, path, aes);
            stepMap.setMoving(path,agent.id());

            if (agent.isInFinishedPath()) {

                runTask((newTask));
                finishTask(newTask);

                agent.unassignTask(*this, aes);
                
//                flag = _stepPath::trivialSteppingPath(path);
//                
//                if(flag){
//                    
//                    agent.assignTrivialPath(*this, path, aes);   
//                    stepMap.setMoving(path,agent.id());
//                    
//                } else {
//                    
//                    try {
//                        std::ostringstream stream;
//                        stream << "invalid trivialSteppingPath: " << std::endl << agent;
//                        MAPD_EXCEPTION(stream.str());
//                    } catch (std::exception& e) {
//                        std::cout << e.what() << std::endl;
//                        std::abort();
//                    }                    
//                    
//                } 
                return false;

            } 

            return true;

        }
        
    }
        
    return false;
    
}

bool _token::updateChargingPathToAgent(_agent& agent, bool energyCheck){
    
    _stepPath chargingPath(agent.currentSite());
    
    bool flag = selectChargingEndpointPathToAgent(agent, chargingPath);
    
    if(flag){
        
        AES aes = AES::none;
        
        if(energyCheck){            
        
            flag = agent.isAbleToFulfillNoCarryngPath(map, chargingPath, aes);
        
        }
        
        if(flag){
        
            agent.assignChargingPath(*this, chargingPath, aes);

            stepMap.setMoving(chargingPath,agent.id());

            return true;
        
        }
        
    }
    
    return false;
    
}

bool _token::updateChargingConflictTaskToAgent(_agent& agent, bool energyCheck){
    
    _task conflict;
    
    _stepPath chargingPath(agent.currentSite());
    
    bool flag = selectChargingEndpointPathToAgent(agent, conflict, chargingPath);
    
    if(flag){
    
        AES aes = AES::none;
        
        if(energyCheck){            

            flag = agent.isAbleToFulfillNoCarryngPath(map, chargingPath, aes);

        }
        
        if(flag){
      
            reportTaskUpdate(agent, conflict, ReportTask::PathType::charging, chargingPath);  

            agent.assignChargingPath(*this, chargingPath, aes);

            stepMap.setMoving(chargingPath,agent.id());

            return true;
        
        }
            
    }
    
    return false;
    
}

bool _token::updateRestPathToAgent(_agent& agent, bool energyCheck = false){
    
    _task conflict;
    
    _stepPath restPath(agent.currentSite());
    
    bool flag = selectNewRestEndpointPathToAgent(agent, conflict, restPath);
    
    if(flag){
        
        AES aes = AES::none;
        
        if(energyCheck){            

            flag = agent.isAbleToFulfillNoCarryngPath(map, restPath, aes);

        }
        
        if(flag){
        
            reportTaskUpdate(agent, conflict, ReportTask::PathType::rest, restPath);  

            agent.assignRestPath(*this, restPath, aes);

            stepMap.setMoving(restPath,agent.id());

            return true;
        
        }
        
    }
    
    return false;
    
}

_token::TokenUpdateType _token::updateChargingPath(_agent& agent){
    
    TokenUpdateType ret = TokenUpdateType::none;
    
    if(agent.isInFinishedPath()){
    
        if(updateChargingTaskPathToAgent(agent)) {
            
            ret = TokenUpdateType::charging_task;
            
        } else {
            
            if(updateChargingPathToAgent(agent)){
                
                ret = TokenUpdateType::charging_rest;
                
            } else {
                
                this->updateChargingTrivialPathToAgent(agent);
                
                ret = TokenUpdateType::charging_trivial;
                
            }
            
        }            
    
    } else {
        
        try {
            std::ostringstream stream;
            stream << "agent not parked: " << std::endl << agent << std::endl<< *this;
            MAPD_EXCEPTION(stream.str());
        } catch (std::exception& e) {
            std::cout << e.what() << std::endl;
            std::abort();
        }
        
    }
    
    return ret;
    
}

_token::TokenUpdateType _token::updatePath(_agent& agent){
    
    TokenUpdateType ret = TokenUpdateType::none;
    
    if(agent.isInFinishedPath()){
    
        if(updateTaskPathToAgent(agent)) {
            
            ret = TokenUpdateType::task;
            
        } else {
            
            if(updateRestPathToAgent(agent)){
                
                ret = TokenUpdateType::rest;
                
            } else {
                
                this->updateTrivialPathToAgent(agent);
                
                ret = TokenUpdateType::trivial;
                
            }
            
        }
            
    
    } else {
        
        try {
            std::ostringstream stream;
            stream << "agent not parked: " << std::endl << agent << std::endl<< *this;
            MAPD_EXCEPTION(stream.str());
        } catch (std::exception& e) {
            std::cout << e.what() << std::endl;
            std::abort();
        }
        
    }
    
    return ret;
    
}


void _token::updateTrivialPathToAgent(_agent& agent, bool energyCheck){
        
    _stepPath trivialPath(agent.currentSite());
             
    bool flag = _stepPath::trivialSteppingPath(trivialPath);
    
    if(flag){
    
        AES aes = AES::none;

        if(energyCheck){

            flag = agent.isAbleToFulfillNoCarryngPath(map, trivialPath, aes);

        }

        if(flag){

            agent.assignTrivialPath(*this, trivialPath, aes);   
            
            stepMap.setMoving(trivialPath, agent.id());

        }
    
    } else {
                    
        try {
            std::ostringstream stream;
            stream << "invalid trivialSteppingPath: " << std::endl << agent;
            MAPD_EXCEPTION(stream.str());
        } catch (std::exception& e) {
            std::cout << e.what() << std::endl;
            std::abort();
        }                    

    }      

}

_token::TokenUpdateType _token::updateChargingTrivialPathToAgent(_agent& agent, bool energyCheck){
           
    _stepPath trivialPath(agent.currentSite());
             
    bool flag = _stepPath::trivialSteppingPath(trivialPath);
    
    if(flag){
        
        AES aes = AES::none;
    
        if(energyCheck){

            flag = agent.isAbleToFulfillNoCarryngPath(map, trivialPath, aes);

        }

        if(flag){

            agent.assignChargingTrivialPath(*this, trivialPath, aes);   
            
            stepMap.setMoving(trivialPath, agent.id());
            
            return _token::TokenUpdateType::charging_trivial;

        }
    
    } else {
                    
        try {
            std::ostringstream stream;
            stream << "invalid trivialSteppingPath: " << std::endl << agent;
            MAPD_EXCEPTION(stream.str());
        } catch (std::exception& e) {
            std::cout << e.what() << std::endl;
            std::abort();
        }                    

    } 
    
    return _token::TokenUpdateType::none;

}

bool _token::updateTaskOrCtaskPathToAgent(_agent& agent, bool energyCheck){
    
    bool c_taskFlag;
    _task origTask;
    _c_task firstC_task, secondC_task;
    _stepPath origTaskPath(agent.currentSite()), firstC_taskPath(agent.currentSite());
    
    bool flag = selectNewTaskOrCtaskPathToAgent(agent, origTask, firstC_task, secondC_task, origTaskPath, firstC_taskPath, c_taskFlag);
       
    if(flag){
        
        AES aes = AES::none;
        
        if(c_taskFlag){
            
            if(energyCheck){

                flag = agent.isAbleToFulfillTaskPath(map, firstC_task, firstC_taskPath, aes);

            } 
            
            if(flag){
                
                agent.assignTask(*this, origTask, origTaskPath, AES::none);
                agent.unassignTask(*this, AES::none);               
                
                reportTaskUpdate(agent, origTask, ReportTask::PathType::task, origTaskPath);                
                assignTask(origTask, agent);
                runTask(origTask);
                finishTask(origTask);
                
                
                firstC_task.setId(this->getOneTaskId());
                secondC_task.setId(this->getOneTaskId());
                
                reportTaskUpdate(agent, firstC_task, ReportTask::PathType::c_task, firstC_taskPath);
                addPendingTask(firstC_task);               
                assignTask(firstC_task, agent);
                
                agent.assignTask(*this, firstC_task, firstC_taskPath, aes);
                stepMap.setMoving(firstC_taskPath, agent.id());
                
                add_c_task(currentStep + firstC_taskPath.size() + 1, secondC_task);
                
            } 
        
        } else {
            
            if(energyCheck){

                flag = agent.isAbleToFulfillTaskPath(map, origTask, origTaskPath, aes);

            } 
            
            if(flag){
                
                reportTaskUpdate(agent, origTask, ReportTask::PathType::task, origTaskPath);
                assignTask(origTask, agent);
                
                agent.assignTask(*this, origTask, origTaskPath, aes);                  
                stepMap.setMoving(origTaskPath, agent.id());                
                
            } 
            
            
        }
        
        if (flag && agent.isInFinishedPath()) {

            agent.unassignTask(*this, aes);

            if(c_taskFlag){

                runTask(firstC_task);
                finishTask(firstC_task);

            }else{

                runTask(origTask);
                finishTask(origTask);

            }
            
//            _stepPath path(agent.currentSite());
//
//            flag = _stepPath::trivialSteppingPath(path);
//                
//            if(flag){
//
//                agent.assignTrivialPath(*this, path, aes);   
//                stepMap.setMoving(path, agent.id());
//
//            } else {
//
//                try {
//                    std::ostringstream stream;
//                    stream << "invalid trivialSteppingPath: " << std::endl << agent;
//                    MAPD_EXCEPTION(stream.str());
//                } catch (std::exception& e) {
//                    std::cout << e.what() << std::endl;
//                    std::abort();
//                }                    
//
//            }      
            
            return false;

        }                 

    }   
    
    return flag;
    
}


// versão 1: partindo de delivery original, escolhe o primeiro task endpoint que 
// atende à razão de fracionamento. 
// Se nenhum endpoint atender, retorna falso.
// Nesse caso, a tarefa não será selecionada.
bool _token::selectNewTaskOrCtaskPathToAgent(const _agent& agent, _task& origTask, _c_task& firstC_task, _c_task& secondC_task, _stepPath& origPath, _stepPath& c_path, bool& c_taskFlag) const{
    
    _thresholdAlgorithm fractioningRateAlgorithm(endpointsDistanceAlgorithm);
    
    unsigned pickupStep;
    bool flag = selectNewTaskPathToAgent(agent, origTask, origPath, pickupStep);
    
    if(flag){
        
        _stepSite pickupSite = origPath.get(pickupStep);
                            
        unsigned carry_size = origPath.goalSite().GetStep() - pickupSite.GetStep();
        
        if(fractioningRateAlgorithm.solve(origPath.goalSite(), pickupSite, carry_size, carry_threshold)){ // verifica se atende ao carry_threshold
            
            c_taskFlag = false;
            
            return true;
            
        } else {
            
            flag = false;        
            
            origPath.backward([&flag, pickupSite, fractioningRateAlgorithm, agent, origTask, &firstC_task, &secondC_task, origPath,  &c_path, &c_taskFlag, this](const _stepSite& site){// voltando

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
                                                        
                            unsigned new_carry_size = site.GetStep() - pickupSite.GetStep();
                            
                            if(fractioningRateAlgorithm.solve(site, pickupSite, new_carry_size, carry_threshold)){ // verifica se o endpoint que atende ao carry_threshold
                                                     
//                                firstC_task.setId(this->getOneTaskId());
                                firstC_task.setPickup(origTask.getPickup());
                                firstC_task.setDelivery(site);
                                firstC_task.SetConcernTaskId(origTask.id());
                                                                           
                                unsigned pkpStep;
                                c_taskFlag = this->taskPathToAgent(agent, firstC_task, c_path, pkpStep);
                                
                                if(c_taskFlag){
                                    
//                                    secondC_task.setId(this->getOneTaskId());
                                    secondC_task.setPickup(site);
                                    secondC_task.setDelivery(origTask.getDelivery());
                                    secondC_task.SetConcernTaskId(origTask.id());
                                    
                                    flag = true;
                                    
                                    return true;
                                    
                                } else {
                                    
                                    try {
                                        std::ostringstream stream;
                                        stream << "unsolved c_task path: " << firstC_task;
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

                return false;

            }, 1);
        
        }
                        
    }
    
    return flag;
    
}