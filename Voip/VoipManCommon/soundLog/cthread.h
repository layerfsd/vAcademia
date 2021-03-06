/*
 * thread.h
 */
#pragma once


///////////////////////////////////////////////////////////////////////////////
// CThread

/** This class defines a thread of execution in the system. A {\it thread} is
   an independent flow of processor instructions. This differs from a
   {\it process} which also embodies a program address space and resource
   allocation. So threads can share memory and resources as they run in the
   context of a given process. A process always contains at least one thread.
   This is reflected in this library by the #PProcess# class being
   descended from the CThread class.

   The implementation of a thread is platform dependent, but it is
   assumed that the platform has some support for native threads.
   Previous versions of PWLib has some support for co-operative
   threads, but this has been removed
 */

typedef int PINDEX;

class CThread
{
public:
  /**@name Construction */
  //@{
    /// Codes for thread priorities.
    enum Priority {
      /// Will only run if all other threads are blocked.
      LowestPriority,   

      /// Runs approximately half as often as normal.
      LowPriority,      

      /// Normal priority for a thread.
      NormalPriority,   

      /// Runs approximately twice as often as normal.
      HighPriority,     

      /// Is only thread that will run, unless blocked.
      HighestPriority,  

      NumPriorities
    };

    /// Codes for thread autodelete flag
    enum AutoDeleteFlag {
      /// Automatically delete thread object on termination.
      AutoDeleteThread,   

      /// Don't delete thread as it may not be on heap.
      NoAutoDeleteThread  
    };

    /** Create a new thread instance. Unless the #startSuspended#
       parameter is TRUE, the threads #Main()# function is called to
       execute the code for the thread.
       
       Note that the exact timing of the execution of code in threads can
       never be predicted. Thus you you can get a race condition on
       intialising a descendent class. To avoid this problem a thread is
       always started suspended. You must call the Resume() function after
       your descendent class construction is complete.

       If synchronisation is required between threads then the use of
       semaphores is essential.

       If the #deletion# is set to #AutoDeleteThread#
       then the CThread is assumed to be allocated with the new operator and
       may be freed using the delete operator as soon as the thread is
       terminated or executes to completion (usually the latter).

       The stack size argument retained only for source code compatibility for
       previous implementations. It is not used in the current code and
       may be removed in subsequent versions.
     */
    CThread(
      PINDEX,                 ///< Not used - previously stack size
      AutoDeleteFlag deletion = AutoDeleteThread,
        ///< Automatically delete CThread instance on termination of thread.
      Priority priorityLevel = NormalPriority,  ///< Initial priority of thread.
      const CComString & threadName = "" ///< The name of the thread (for Debug/Trace)
    );

    /** Destroy the thread, this simply calls the #Terminate()# function
       with all its restrictions and penalties. See that function for more
       information.

       Note that the correct way for a thread to terminate is to return from
       the #Main()# function.
     */
    ~CThread();
  //@}

  /**@name Control functions */
  //@{
    /** Restart a terminated thread using the same stack priority etc that
       was current when the thread terminated.
       
       If the thread is still running then this function is ignored.
     */
    virtual void Restart();

    /** Terminate the thread. It is highly recommended that this is not used
       except in abnormal situations as not all clean up of resources
       allocated to the thread will be executed. This is especially true in
       C++ as the destructors of objects that are automatic variables are not
       called causing at the very least the possiblity of memory leaks.

       Note that the correct way for a thread to terminate is to return from
       the #Main()# function or self terminate by calling
       #Terminate()# within the context of the thread which can then
       assure that all resources are cleaned up.
     */
    virtual void Terminate();

    /** Determine if the thread has been terminated or ran to completion.

       @return
       TRUE if the thread has been terminated.
     */
    virtual BOOL IsTerminated() const;

    /** Block and wait for the thread to terminate.

       @return
       FALSE if the thread has not terminated and the timeout has expired.
     */
    void WaitForTermination() const;
    BOOL WaitForTermination(
      const long maxWait  ///< Maximum time to wait for termination.
    ) const;

    /** Suspend or resume the thread.
    
       If #susp# is TRUE this increments an internal count of
       suspensions that must be matched by an equal number of calls to
       #Resume()# or #Suspend(FALSE)# before the
       thread actually executes again.

       If #susp# is FALSE then this decrements the internal count of
       suspensions. If the count is <= 0 then the thread will run. Note that
       the thread will not be suspended until an equal number of
       #Suspend(TRUE)# calls are made.
     */
    virtual void Suspend(
      BOOL susp = TRUE    ///< Flag to suspend or resume a thread.
    );

    /** Resume thread execution, this is identical to
       #Suspend(FALSE)#.

      The Resume() method may be called from within the constructor of a
      CThread descendant.  However, the Resume() should be in the
      constructor of the most descendant class. So, if you have a
      class B (which is descended of CThread), and a class C (which is
      descended of B), placing the call to Resume in the constructor of B is
      unwise.

      If you do place a call to Resume in the constructor, it
      should be at the end of the constructor, after all the other
      initialisation in the constructor.

      The reason the call to Resume() should be at the end of the
      construction process is simple - you want the thread to start
      when all the variables in the class have been correctly
      initialised.
     */
    virtual void Resume();

    /** Determine if the thread is currently suspended. This checks the
       suspension count and if greater than zero returns TRUE for a suspended
       thread.

       @return
       TRUE if thread is suspended.
     */
    virtual BOOL IsSuspended() const;

    /// Suspend the current thread for the specified amount of time.
    static void Sleep(
      const long delay   ///< Time interval to sleep for.
    );

    /** Set the priority of the thread relative to other threads in the current
       process.
     */
    virtual void SetPriority(
      Priority priorityLevel    ///< New priority for thread.
    );

    /** Get the current priority of the thread in the current process.

       @return
       current thread priority.
     */
    virtual Priority GetPriority() const;

    /** Set the flag indicating thread object is to be automatically deleted
        when the thread ends.
     */
    virtual void SetAutoDelete(
      AutoDeleteFlag deletion = AutoDeleteThread  ///< New auto delete setting.
    );

    /** Reet the flag indicating thread object is to be automatically deleted
        when the thread ends.
     */
    void SetNoAutoDelete() { SetAutoDelete(NoAutoDeleteThread); }

    /** Get the name of the thread. Thread names are a optional debugging aid.

       @return
       current thread name.
     */
    virtual CComString GetThreadName() const;

    /** Change the name of the thread. Thread names are a optional debugging aid.

       @return
       current thread name.
     */
    virtual void SetThreadName(
      CComString & name        ///< New name for the thread.
    );
  //@}

  /**@name Miscellaneous */
  //@{
    /** Get operating system specific thread identifier for this thread.
      * Note that the return value from these functions is only valid
      * if called by the owning thread. Calling this function for another
      * thread that may be terminating is a very bad idea.
      */
    virtual DWORD GetThreadId() const;
    static DWORD GetCurrentThreadId();

    /** User override function for the main execution routine of the thread. A
       descendent class must provide the code that will be executed in the
       thread within this function.
       
       Note that the correct way for a thread to terminate is to return from
       this function.
     */
    virtual void Main() = 0;

    /** Get the currently running thread object instance. It is possible, even
       likely, that the smae code may be executed in the context of differenct
       threads. Under some circumstances it may be necessary to know what the
       current codes thread is and this static function provides that
       information.

       @return
       pointer to current thread.
     */
    static CThread * Current();

    /**Create a simple thread executing the specified notifier.
       This creates a simple CThread class that automatically executes the
       function defined by the PNotifier in the context of a new thread.
      */
/*    static CThread * Create(
      const PNotifier & notifier,     ///< Function to execute in thread.
      INT parameter = 0,              ///< Parameter value to pass to notifier.
      AutoDeleteFlag deletion = AutoDeleteThread,
        ///< Automatically delete CThread instance on termination of thread.
      Priority priorityLevel = NormalPriority,  ///< Initial priority of thread.
      const CComString & threadName = CComString::Empty(), ///< The name of the thread (for Debug/Trace)
      PINDEX stackSize = 10000         ///< Stack size on some platforms
    );*/
  //@}

protected:
	void InitialiseProcessThread();
    /* Initialialise the primordial thread, the one in the PProcess. This is
       required due to the bootstrap logic of processes and threads.
     */

private:
	CThread();
    // Create a new thread instance as part of a PProcess class.    

	CThread(const CThread &) { }
    // Empty constructor to prevent copying of thread instances.

	CThread & operator=(const CThread &) { return *this; }
    // Empty assignment operator to prevent copying of thread instances.

	BOOL autoDelete;
    // Automatically delete the thread on completion.

    // Give the thread a name for debugging purposes.
	CComString threadName;

private:
	unsigned traceBlockIndentLevel;

public:
	HANDLE GetHandle() const { return threadHandle; }
	void Win32AttachThreadInput();

protected:
	HANDLE threadHandle;
	UINT   threadId;

private:
	PINDEX originalStackSize;

	static DWORD WINAPI MainFunction(LPVOID thread);
};

// End Of File ///////////////////////////////////////////////////////////////
