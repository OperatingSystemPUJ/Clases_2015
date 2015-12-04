int32_t start_snort_instance(snort_context_t *ctx) {
    pid_t pid;
    int32_t cpu;
    
    if ((cpu = get_cpu_affinity()) == -1) {
        error("%s: get_cpu_affinity() failed: %s", __FUNCTION__, strerror(errno));
        return -1;
    }

    switch (pid = fork()) {
        case -1: {
            error("%s: fork() failed: %s", __FUNCTION__, strerror(errno));
            return -1;
            break;
        }
        case 0: {
            int32_t status;
            char *argv[18];
            const char *envp[] = {
                "LD_LIBRARY_PATH=/usr/lib:/lib:/usr/local/lib:$LD_LIBRARY_PATH",
                NULL
            };

            for (uint32_t i = 0; i < 17; i++) {
                if ((argv[i] = (char *)malloc(MAX_STR)) == NULL) {
                    error("%s: Failed to allocate memory for snort params", __FUNCTION__);
                    return -1;
                }
            }

            snprintf(argv[0], MAX_STR, "%s", ctx->snort_bin);
            snprintf(argv[1], MAX_STR, "--daq-dir");
            snprintf(argv[2], MAX_STR, "%s", ctx->daq_dir);
            snprintf(argv[3], MAX_STR, "--daq");
            snprintf(argv[4], MAX_STR, "%s", ctx->daq_module);
            snprintf(argv[5], MAX_STR, "--daq-mode");
            snprintf(argv[6], MAX_STR, "read-file");
            snprintf(argv[7], MAX_STR, "--daq-var");            
            snprintf(argv[8], MAX_STR, "cpu=%d", cpu);
            snprintf(argv[9], MAX_STR, "-G");
            snprintf(argv[10], MAX_STR, "%d", getpid());
            snprintf(argv[11], MAX_STR, "-c"); 
            snprintf(argv[12], MAX_STR, "%s", ctx->snort_conf);
            snprintf(argv[13], MAX_STR, "-l");
            snprintf(argv[14], MAX_STR, "%s/snort%d", ctx->snort_bin_log_dir, cpu);
            snprintf(argv[15], MAX_STR, "--pid-path");            
            snprintf(argv[16], MAX_STR, "%s/snort%d", ctx->snort_pid_path, cpu);
            argv[17] = NULL;

            char snort_cline[2048] = {0};
            for (uint32_t i = 0; i < 17; i++) {
                strcat(snort_cline, argv[i]);
                strcat(snort_cline, " ");
            }
            debug("%s: Snort command line is:\n%s", __FUNCTION__, snort_cline);

            // Append PID to aggregated PID file
            info("%s: Adding %d to snort aggregate pid file", __FUNCTION__, getpid());
            FILE *fp;
            if ((fp = fopen(ctx->snort_aggr_pid_file, "a+")) == NULL) {
                error("%s: Could not open snort aggregated log file: %s", 
                        __FUNCTION__, ctx->snort_aggr_pid_file);
                return BAL_ERROR;
            }
            fprintf(fp, "%d\n", getpid());
            fclose(fp);
        
            //Redirect snort output
            char fname[PATH_MAX];
            snprintf(fname, PATH_MAX - 1, "%s/snort_%d.log", ctx->snort_log_dir, cpu);
            info("%s: Redirecting snort log to: %s", __FUNCTION__, fname);
            int32_t fd = open(fname, O_RDWR | O_CREAT | O_APPEND, S_IRUSR | S_IWUSR);
            dup2(fd, 1);
            dup2(fd, 2);
            close(fd);

            if (mkdir(argv[14], 0700) < 0) {
                if (errno != EEXIST) {
                    error("%s: mkdir() failed: %s", __FUNCTION__, strerror(errno));
                    return -1;
                }
            }

            status = execve(ctx->snort_bin, argv, (char * const *) envp);

            for (uint32_t i = 0; i < 17; i++) {
                free(argv[i]);
            }

            perror("execve()");
            exit(status);
            break;
        }
        default:
            return pid;
    }

    return 0;
}