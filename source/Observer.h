#ifndef OBSERVER_H
#define OBSERVER_H

#include <vector>

// This is a basic observer class, although it is a standard implementation,
// I copied the code from: http://sourcemaking.com/design_patterns/observer/cpp/1
//		note: the code I copied needed to be fixed, vector class used incorrectly.
// TODO: extend this to use weak pointers.

// To use, an observer must implement the "Observer" template.
// The subject must implement the "Subject" template.

// Once these are done, the Observer must be attached to the Subject via:
//		Subject sub;
//		Observer obs;
//		sub.attach(obs);

// At this point the observer may be notified of any action via
// a call to sub.notify(), or sub.set_val(int value)
class Observer
{
  public:
    virtual void update(int value) = 0;
};

class Subject
{
    int m_value;
    std::vector<Observer*> m_views;
  public:
    void attach(Observer *obs)
    {
        m_views.push_back(obs);
    }

		// remove an observer.
		// TODO: test this, I added it on my own.
		void detach(Observer *obs)
		{
			for (unsigned int i = 0; i < m_views.size(); ++i)
				if ( m_views[i] == obs )
					m_views.erase( m_views.begin() + i, m_views.begin() + i + 1 );
		}
    void set_val(int value)
    {
        m_value = value;
        notify();
    }
    void notify()
    {
        for (unsigned int i = 0; i < m_views.size(); ++i)
          m_views[i]->update(m_value);
    }
};


#endif
