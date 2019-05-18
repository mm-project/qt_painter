#ifndef service_h
#define service_h

template <typename T>
class Service
{
    public:
        //static T* get();
        static T* get_instance() 
        {
            if(!m_instance) m_instance = new T;
            return m_instance;
        }

    private:
        static T* m_instance;
};

template <typename T>
T* Service<T>::m_instance = 0;

#endif
