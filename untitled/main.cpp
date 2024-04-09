#include <deque>
#include <memory>
#include <functional>
#include <pthread.h>

class thread_pool_t {
public:
    template<typename T>
    class future_t {
        std::shared_ptr<T> result;
        std::shared_ptr<bool> ready;
        std::shared_ptr<pthread_mutex_t> m_mutex;
        std::shared_ptr<pthread_cond_t> m_condition;

        friend class thread_pool_t;

        future_t() {
            result = std::make_shared<T>();
            ready = std::make_shared<bool>(false);
            m_mutex = std::make_shared<pthread_mutex_t>();
            m_condition = std::make_shared<pthread_cond_t>();

            pthread_mutex_init(m_mutex.get(), nullptr);
            pthread_cond_init(m_condition.get(), nullptr);
        }

    public:
        T get() {
            if(!(*ready)) {
                pthread_mutex_lock(m_mutex.get());
                while (!(*ready)) {
                    pthread_cond_wait(m_condition.get(), m_mutex.get());
                }
                pthread_mutex_unlock(m_mutex.get());
            }
            return *result;
        }
    };

private:
    template<typename T>
    struct packed_task_t {
        std::function<T()> task;
        future_t<T> future;
    };

    struct thread_arg_t {
        std::deque<std::function<void()>>* tasks_queue;
        pthread_mutex_t* mutex;
        pthread_cond_t* condition;
        bool* stop;
    };

    std::deque<std::function<void()>> m_tasks;
    std::vector<pthread_t> m_threads;

    pthread_mutex_t m_mutex = PTHREAD_MUTEX_INITIALIZER;
    pthread_cond_t m_condition = PTHREAD_COND_INITIALIZER;

    bool m_stop = false;

    static void* thread_routine(void* arg) {
        auto* thread_arg = static_cast<thread_arg_t*>(arg);
        while (true) {
            std::function<void()> task;

            pthread_mutex_lock(thread_arg->mutex);
            while (thread_arg->tasks_queue->empty() && !(*thread_arg->stop)) {
                pthread_cond_wait(thread_arg->condition, thread_arg->mutex);
            }

            if (thread_arg->tasks_queue->empty() && *thread_arg->stop) {
                pthread_mutex_unlock(thread_arg->mutex);
                break;
            }

            task = std::move(thread_arg->tasks_queue->front());
            thread_arg->tasks_queue->pop_front();
            pthread_mutex_unlock(thread_arg->mutex);

            task();
        }

        delete thread_arg;
        return nullptr;
    }


public:
    explicit thread_pool_t(std::size_t num_threads) {
        m_threads.reserve(num_threads);

        for (size_t i = 0; i < num_threads; ++i) {
            auto arg = new thread_arg_t{&m_tasks, &m_mutex, &m_condition, &m_stop};
            pthread_t thread;
            pthread_create(&thread, nullptr, thread_routine, arg);
            m_threads.push_back(thread);
        }
    }

    template<typename T>
    auto submit(std::function<T()>& task) -> future_t<T> {
        auto wrapper = std::make_shared<packed_task_t<T>>();
        wrapper->task = std::move(task);

        pthread_mutex_lock(&m_mutex);
        m_tasks.emplace_back([wrapper] {
            *(wrapper->future.result) = wrapper->task();
            *(wrapper->future.ready) = true;

            pthread_mutex_lock(wrapper->future.m_mutex.get());
            pthread_cond_broadcast(wrapper->future.m_condition.get());
            pthread_mutex_unlock(wrapper->future.m_mutex.get());
        });
        pthread_mutex_unlock(&m_mutex);
        pthread_cond_broadcast(&m_condition);

        return wrapper->future;
    }

    ~thread_pool_t() {
        pthread_mutex_lock(&m_mutex);
        m_stop = true;
        pthread_mutex_unlock(&m_mutex);

        pthread_cond_broadcast(&m_condition);

        for (auto& thread : m_threads) {
            pthread_join(thread, nullptr);
        }

        pthread_mutex_destroy(&m_mutex);
        pthread_cond_destroy(&m_condition);
    }
};

#include <unistd.h>

int main() {
    thread_pool_t tpool(4);

    std::vector<thread_pool_t::future_t<int>> results;

    for(int i = 0; i < 10; ++i) {
        std::function<int()> f = [] {
            sleep(1);
            return 42;
        };

        results.push_back(tpool.submit<int>(f));
    }

    for(auto& result : results) {
        std::cout << result.get() << std::endl;
    }

    return 0;
}