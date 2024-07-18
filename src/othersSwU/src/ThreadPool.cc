#include "ThreadPool.h"
#include <chrono>

bool runThreadPoolTest()
{
    LOG_INFO << "ThreadPoolTest run test";

    ThreadPool pool(4);
    std::vector< std::future<int> > results;

    for(int i = 0; i < 8; ++i) {
        results.emplace_back(
            pool.enqueue([i](){
                std::cout << "hello " << i << std::endl;
                std::this_thread::sleep_for(std::chrono::seconds(1));
                std::cout << "world " << i << std::endl;
                return i*i;
            })
        );
    }
    LOG_INFO << "ThreadPoolTest enqueue done";

    for(auto && result: results)
        std::cout << "----: " << result.get() << " ";  // // 阻塞，直到计算完成并获取结果
    std::cout << std::endl;

    return true;
}

namespace nm_ThreadPoolTest{
      // bool _=runThreadPoolTest();
}