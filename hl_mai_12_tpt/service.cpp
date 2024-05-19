#include <iostream>
#include <cstdlib>
#include <map>
#include "httplib.h"

using data_t = std::map<std::string, std::string>;

data_t values;

/**
 * Transaction data storage
 * tx_id -> {user_id,value} {user_id,value}
 */
std::map<std::string, data_t> txs;

/**
 * Transaction status
 * {created, prepared, doubt,commited, aborted}
 */
std::map<std::string,std::string> txs_status;

auto main() -> int
{
    httplib::Server svr;

    /**
     * @brief Change Data
     *
     * id - key
     * value - new value
     * tx - transaction id
     */
    svr.Get(R"(/change_user/(\d+))", [](const httplib::Request &req, httplib::Response &res)
            {
                auto numbers = req.matches[1];
                std::string id = numbers;
    
                if (req.has_param("value")&&req.has_param("tx")) {
                        std::string val = req.get_param_value("value");
                        std::string tx  = req.get_param_value("tx");

                        std::cout << "{ id=" << id ;
                        std::cout << ", tx=" << tx ;
                        std::cout << ", value=" << val << "}" << std::endl;

                        // find transaction
                        if(txs.find(tx)!=std::end(txs)) {
                            data_t &tx_data = txs[tx];
                            tx_data[id] = val;
                            std::cout <<"update transaction data tx=" << tx << std::endl;
                        } else {
                            data_t tx_data;
                            tx_data[id]=val;
                            txs[tx] = tx_data;
                            std::cout <<"create transaction data tx=" << tx << std::endl;
                        }
                        res.status = 200;
                        res.set_content(val, "text/plain"); 
                    } else {
                        res.status = 404;
                        res.set_content(req.body, "text/plain"); 
                    } });
    /**
     * @brief Get Data
     * id - key
     */

    svr.Get(R"(/get_user/(\d+))", [](const httplib::Request &req, httplib::Response &res)
            {
                auto numbers = req.matches[1];
                std::string id = numbers;
                std::string result;
                std::string value;

                
                for(auto &[tx,data] : txs){
                    if(txs_status[tx]=="prepared") //dirty read
                    if(data.find(id)!=std::end(data)){
                        value = data[id];
                    }
                } 

                // check if there is data in local storage
                if(!value.empty()){
                    res.status = 200;
                    result = "{'value' : '" + value + "' }";
                    res.set_content(result, "text/plain"); 
                } else
                // check if there is data in main storage
                if(values.find(id)!=std::end(values)){
                    res.status = 200;
                    result = "{'value' : '" + values[id] + "' }";
                    res.set_content(result, "text/plain"); 
                } else {
                    res.status = 404;
                    res.set_content("", "text/plain"); 
                }

                });

    /**
     * @brief Prepare transaction
     * tx - transaction
     */
    svr.Get(R"(/prepare/(\d+))", [](const httplib::Request &req, httplib::Response &res)
            {
                auto numbers = req.matches[1];
                std::string tx = numbers;
                std::cout << "prepare {tx=" << tx << "}" << std::endl;
                if(txs.find(tx)!=std::end(txs)){
                    txs_status[tx] = "prepared";
                    std::cout <<"prepare transaction data tx=" << tx << std::endl;
                    res.status = 200;
                    res.set_content("{'result' : 'prepared'}", "text/plain");
                }
            });

    /**
     * @brief Doubt transaction
     * tx - transaction
     */
    svr.Get(R"(/doubt/(\d+))", [](const httplib::Request &req, httplib::Response &res)
            {
                auto numbers = req.matches[1];
                std::string tx = numbers;
                std::cout << "doubt {tx=" << tx << "}" << std::endl;
                if(txs.find(tx)!=std::end(txs)){
                    txs_status[tx] = "doubt";
                    std::cout <<"doubt transaction data tx=" << tx << std::endl;
                    res.status = 200;
                    res.set_content("{'result' : 'in doubt'}", "text/plain");
                }
            });
    /**
     * @brief Abort transaction
     * tx - transaction
     */
    svr.Get(R"(/abort/(\d+))", [](const httplib::Request &req, httplib::Response &res)
            {
                auto numbers = req.matches[1];
                std::string tx = numbers;
                std::cout << "abort {tx=" << tx << "}" << std::endl;
                if(txs.find(tx)!=std::end(txs)){
                    txs.erase(tx);
                    txs_status[tx] = "aborted";
                    std::cout <<"erase transaction data tx=" << tx << std::endl;
                    res.status = 200;
                    res.set_content("{'result' : 'aborted'}", "text/plain");
                }
            });

    /**
     * @brief Commit transaction
     * tx - transaction
     */
    svr.Get(R"(/commit/(\d+))", [](const httplib::Request &req, httplib::Response &res)
            {
                auto numbers = req.matches[1];
                std::string tx = numbers;
                std::cout << "commit {tx=" << tx << "}" << std::endl;
                if(txs.find(tx)!=std::end(txs)){
                    // check transaction state
                    if(txs_status.find(tx)==std::end(txs_status)||(txs_status[tx]!="prepared")){
                        res.status = 404;
                        res.set_content("{'result' : 'transaction not prepared'}", "text/plain");
                    }

                    data_t &dt = txs[tx];
                    for (auto &[id, val] : dt)
                    {
                        values[id] = val;
                    }
                    txs_status[tx] = "commited";
                    std::cout <<"commit transaction data tx=" << tx << std::endl;
                    res.status = 200;
                    res.set_content("{'result' : 'commited'}", "text/plain");
                }
            });
    
    std::cout << "starting server .." << std::endl;
    svr.listen("0.0.0.0", 8080);
    return 0;
}