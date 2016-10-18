pthread_t loopthread, watchdogthread;

char watchdog_check = 1;

int pthread_create_realtime(
    pthread_t *new_thread,
	void *(*start)(void *),
    void *arg,
	int priority
) {
    // nicked from Paul Barton-Davis' Ardour code :)
    pthread_attr_t *rt_attributes;
	struct sched_param *rt_param;
	int retval;
	rt_attributes = (pthread_attr_t *) malloc (sizeof (pthread_attr_t));
	rt_param = (struct sched_param *) malloc (sizeof (struct sched_param));
	pthread_attr_init (rt_attributes);
	if (seteuid (0)) {
        sprintf(stderr, "Cannot obtain root UID for RT scheduling operations\n");
		return -1;
	} else {
	    if (pthread_attr_setschedpolicy (rt_attributes, SCHED_FIFO)) {
            sprintf(stderr, "Cannot set FIFO scheduling attributes for RT thread\n");
	    }
	    if (pthread_attr_setscope (rt_attributes, PTHREAD_SCOPE_SYSTEM)) {
            sprintf(stderr, "Cannot set scheduling scope for RT thread\n");
	    }
	    rt_param->sched_priority = priority;
	    if (pthread_attr_setschedparam (rt_attributes, rt_param)) {
            sprintf(stderr, "Cannot set scheduling priority for RT thread\n");
	    }
	}
	retval = pthread_create (new_thread, rt_attributes, start, arg);
	seteuid (getuid());
	return retval;
}


void watchdog (void *arg)
{
	pthread_setcanceltype (PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
	watchdog_check = 0;
	while (1) {
		usleep (10000000);
		if (watchdog_check == 0) {
            fprintf(stderr, "watchdog: timeout - killing synth\n";
			if (watchdog_check == 0) {
                fprintf(stderr, "diagnosis: audio hung?\n");
            }
			exit (1);
		}
		watchdog_check = 0;
	}
}

void audioloop(void* o)
{
    audio_loop:
		if (!synth->CallbackMode()) {
			// do funky stuff
			synth->Update();
			// put the brakes on if there is no blocking output running
			if (!synth->IsBlockingOutputPluginReady() || synth->IsFrozen()) {
				usleep(10000);
			}
		} else {
			// the engine is currently in callback mode, so we don't
			// need to do anything unless we are switched back
			usleep(1000000);
		}
		watchdog_check = 1;
        goto audio_loop;
}

void spawnAudioThread(int Realtime) {
    // spawn the audio thread
    if (Realtime) {
        pthread_create_realtime(
            &watchdogthread,
            (void*(*)(void*))watchdog,
            NULL,
            sched_get_priority_max(SCHED_FIFO)
        );
        pthread_create_realtime(
            &loopthread,
            (void*(*)(void*))audioloop,
            NULL,
            sched_get_priority_max(SCHED_FIFO)-1
        );
    } else {
        pthread_create(
            &loopthread,
            NULL,
            (void*(*)(void*))audioloop,
            NULL
        );
        // reduce the priority of the gui
        if (setpriority(PRIO_PROCESS, 0, 20)) {
            fprintf(stderr, "Could not set priority for main thread\n";
        }
    }
}

