#include "Agent.h"
#include "Token.h"
#include "Render.h"
#include "Circle.h"
#include "Text.h"

void Agent::receive(Token& token) {
 
    this->updatePath(token);
    
}

void Agent::updatePath(Token& token){

    if (state == free) {

        if (this->isResting()) {

            if (selectNewTask(token)) {

                updateTaskPath(token);
                state == occupied;

            } else if (isParked()) {

                if (isConflictingRestEndpoint(token)) {

                    updateRestEndpointPath(token);

                } else {

                    updateTrivialPath(token);

                }

            }

        }
        
    }

}

void Agent::move(Token& token) {

    if (currentPath.size() > 1) {

        currentPath.pop();
        
        moveUpdate(token);

    } else {

        try {
            std::ostringstream stream;
            stream << "invalid path size";
            MAPD_EXCEPTION(stream.str());
        } catch (std::exception& e) {
            std::cout << e.what() << std::endl;
            std::abort();
        }

    }

}

void Agent::moveUpdate(Token& token) {
    
    if (state == free) {

        if(this->isPickupping()){
            
            this->state = occupied;
            
        }        

    } else if (state == occupied) {

        if(this->isDelivering()){
            
            token.removeOpenTask(*currentTask);
            delete currentTask;
            currentTask = nullptr;
            this->state = free;
            
        }

    }    

}

void Agent::updateTrivialPath(Token& token) {

    if (currentPath.size() == 1) {

        _stepSite site = currentPath.currentSite();
        site.SetStep(site.GetStep() + 1);
        currentPath.progress(site);
        token.getIntegerMap().setMoving(currentPath, this->id());

    } else {

        try {
            std::ostringstream stream;
            stream << "unsolved trivial path";
            MAPD_EXCEPTION(stream.str());
        } catch (std::exception& e) {
            std::cout << e.what() << std::endl;
            std::abort();
        }

    }

}

_site* Agent::selectNewEndpoint(Token& token) {

    unsigned min_distance = 0xffffffff;
    _site* endpoint = nullptr;

    token.listEndpoints([&token, &min_distance, &endpoint, this](const _site & site) {

        token.listTasks([&token, &min_distance, &endpoint, site, this](const Task & task) {

            if (!task.getDelivery().match(site)) {

                token.listAgents([&token, &min_distance, &endpoint, site, task, this](Agent & agent) {

                    if (this->id() != agent.id()) { //other agents

                        if (!agent.goalSite().match(site)) {

                            unsigned distance = token.distanceAlgorithm()->solve(this->currentSite(), site);

                            if (distance < min_distance) {
                                min_distance = distance;
                                if (endpoint != nullptr) delete endpoint;
                                endpoint = new _site(site);
                            }

                        }

                    } else {

                        unsigned distance = token.distanceAlgorithm()->solve(this->currentSite(), site);

                        if (distance < min_distance) {
                            min_distance = distance;
                            if (endpoint != nullptr) delete endpoint;
                            endpoint = new _site(site);
                        }

                    }

                    return false;

                });


            }

            return false;

        });


        return false;

    });

    return endpoint;

}

bool Agent::selectNewTask(Token& token) {


    unsigned min_distance = 0xffffffff;
    Task selectedTask;

    token.listTasks([&token, &min_distance, &selectedTask, this](const Task & task) {

        token.listAgents([&token, &min_distance, &selectedTask, task, this](const Agent & agent) {

            if (this->id() != agent.id()) { //other agents

                if (!agent.goalSite().match(task.getPickup()) && !agent.goalSite().match(task.getDelivery())) {

                    unsigned distance = token.distanceAlgorithm()->solve(this->currentSite(), task.getPickup());

                    if (distance < min_distance) {
                        min_distance = distance;
                        selectedTask = task;
                    }

                }

            }

            return false;

        });

        return false;

    });

    if (min_distance < 0xffffffff) {

        if (currentTask != nullptr) delete currentTask;
        currentTask = new Task(selectedTask);

        token.removePendingTask(selectedTask);
        token.addOpenTask(selectedTask);

        return true;

    }



    return false;

}

bool Agent::isConflictingRestEndpoint(Token& token) const {

    bool conflit = false;

    token.listTasks([token, &conflit, this](const Task & task) {

        if (task.getDelivery().match(this->currentSite())) {

            conflit = true;

            return true;

        }

        return false;

    });

    return conflit;

}

void Agent::updateRestEndpointPath(Token& token) {

    _site* endpoint = selectNewEndpoint(token);

    if (endpoint != nullptr) {

        _stepPath restPath;

        bool flag = token.pathAlgorithm()->solve(
                token.getIntegerMap(),
                this->currentPath.goalSite(),
                *endpoint,
                restPath,
                this->currentPath.goalSite().GetStep(), this->id());


        delete endpoint;

        if (flag) {

            restPath.pop();

            if (restPath.size() > 0) {

                token.reportTaskUpdate(currentTask->id(), this->id(), ReportTask::PathType::rest, restPath);
                currentPath.progress(restPath);
                token.getIntegerMap().setMoving(currentPath, this->id());


            }

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
            stream << "new rest endpoint not found";
            MAPD_EXCEPTION(stream.str());
        } catch (std::exception& e) {
            std::cout << e.what() << std::endl;
            std::abort();
        }

    }


}


void Agent::updateTaskPath(Token& token) {

    _stepPath pickupPath;
    bool flag = token.pathAlgorithm()->solve(
            token.getIntegerMap(),
            this->currentPath.currentSite(),
            currentTask->getPickup(),
            pickupPath,
            token.getCurrentStep(), this->id());

    if (flag) {

        token.reportTaskUpdate(currentTask->id(), this->id(), ReportTask::PathType::pickup, pickupPath);
        pickupPath.pop();
        currentPath.progress(pickupPath);

        _stepPath deliveryPath;
        flag = token.pathAlgorithm()->solve(
                token.getIntegerMap(),
                currentTask->getPickup(),
                currentTask->getDelivery(),
                deliveryPath,
                token.getCurrentStep() + currentPath.size() - 1, this->id());

        if (flag) {

            token.reportTaskUpdate(currentTask->id(), this->id(), ReportTask::PathType::delivery, deliveryPath);

            deliveryPath.pop();
            currentPath.progress(deliveryPath);
            token.getIntegerMap().setMoving(currentPath, this->id());


        } else {

            try {
                std::ostringstream stream;
                stream << "unsolved task delivery endpoint path: " << *currentTask;
                MAPD_EXCEPTION(stream.str());
            } catch (std::exception& e) {
                std::cout << e.what() << std::endl;
                std::abort();
            }

        }

    } else {

        try {
            std::ostringstream stream;
            stream << "unsolved task pickup endpoint path: " << *currentTask;
            MAPD_EXCEPTION(stream.str());
        } catch (std::exception& e) {
            std::cout << e.what() << std::endl;
            std::abort();
        }

    }

}


void Agent::draw(const Render& render) const {

    sf::Vector2f position(
            this->currentSite().GetColunm() * render.GetCell().first,
            this->currentSite().GetRow() * render.GetCell().second);

    Circle background(
            position,
            sf::Vector2f(render.GetCell().first / 2, 0),
            sf::Color::Black);

    Text textAgentId(
            std::to_string(this->id()),
            position,
            sf::Vector2f(
            render.GetCell().first / 2,
            0),
            sf::Color::White);

    textAgentId.draw(render);

    background.draw(render);
    textAgentId.draw(render);

    if (this->currentTask != nullptr) {

        Text textTaskId(
                std::to_string(currentTask->id()),
                position + sf::Vector2f(render.GetCell().first / 2, render.GetCell().first / 2),
                sf::Vector2f(
                render.GetCell().first / 2,
                0),
                sf::Color::Cyan);

        textTaskId.draw(render);

        if (this->state == occupied) {

            Circle occupied(
                    position + sf::Vector2f(render.GetCell().first / 2, 0),
                    sf::Vector2f(render.GetCell().first / 4, 0),
                    sf::Color::Cyan);

            occupied.draw(render);


        }

    }



}





