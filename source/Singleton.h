#ifndef SINGLETON_H
#define SINGLETON_H

// Used clasic singleton pattern.
// This particular implementation was taken from:
//  http://www.yolinux.com/TUTORIALS/C++Singleton.html

template <class T>
class Singleton
{
public:
  static T* getInstance() {
      if(!m_pInstance) m_pInstance = new T;
      //assert(m_pInstance !='\0');
      return m_pInstance;
  }
protected:
  Singleton();
  ~Singleton();
private:
  Singleton(Singleton const&);
  Singleton& operator=(Singleton const&);
  static T* m_pInstance;
};

template <class T> T* Singleton<T>::m_pInstance='\0';
// template usage, global definition:
//typedef Singleton<GameWorld> GameWorldSingleton

#endif
