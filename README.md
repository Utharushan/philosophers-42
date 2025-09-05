🍝 Philosophers  
An implementation of the classic Dining Philosophers problem, developed as part of the 42 School curriculum.

🧠 Introduction  
Philosophers is a project focused on concurrency and synchronization.  
It simulates philosophers sitting around a table, alternating between eating, sleeping, and thinking — while competing for limited forks.  
The challenge is to manage resources, avoid deadlocks, and ensure accurate timing.

🚀 Features  
✅ Mandatory:  
- Simulates philosophers with threads (one per philosopher)  
- Forks protected by mutexes to avoid data races  
- Monitoring system to detect starvation and death  
- Deterministic logging of state changes (no interleaved prints)  

✅ Bonus (if implemented):  
- Process-based version with semaphores (`philo_bonus`)  
- Independent philosopher processes  
- Semaphore-based synchronization for forks and printing  

🧪 Usage  
Build with:  
```bash
make
```

Run:
```bash
./philo number_of_philosophers time_to_die time_to_eat time_to_sleep [number_of_times_each_must_eat]
```

Example:
```bash
./philo 5 800 200 200
./philo 5 800 200 200 7
```

📚 Implementation Highlights
🔧 Threads & Mutexes

One thread per philosopher

Mutex per fork to avoid race conditions

Monitor thread to detect death or full meals

⏱ Timing

Uses a custom sleep function to ensure millisecond precision

Logs every action with a timestamp since the start

🛡 Deadlock Prevention

Fork locking order avoids circular waits

Odd/even philosophers staggered at startup to reduce contention
