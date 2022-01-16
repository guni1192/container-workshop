#define _GNU_SOURCE

#include <error.h>
#include <fcntl.h>
#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mount.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

// new_uidmap -- HostのUIDをコンテナのUID 0(root user)に割り当てる
void new_uidmap(uid_t uid) {
    int fd = open("/proc/self/uid_map", O_WRONLY);
    if (fd < 0) {
        perror("open /proc/self/uid_map");
        exit(EXIT_FAILURE);
    }

    char uid_map[255];
    sprintf(uid_map, "0 %d 1", (int)uid);

    int ret = write(fd, uid_map, sizeof(uid_map));
    if (ret != sizeof(uid_map)) {
        perror("write /proc/self/uid_map");
        exit(EXIT_FAILURE);
    }

    close(fd);
}

// new_gidmap -- HostのGIDをコンテナのGID 0(root group)に割り当てる
void new_gidmap(gid_t gid) {
    int fd = open("/proc/self/setgroups", O_WRONLY);
    if (fd < 0) {
        perror("open /proc/self/setgroups");
        exit(EXIT_FAILURE);
    }
    int ret = write(fd, "deny", sizeof("deny"));
    if (ret != sizeof("deny")) {
        perror("write /proc/self/setgroups");
        exit(EXIT_FAILURE);
    }
    close(fd);

    fd = open("/proc/self/gid_map", O_WRONLY);
    if (fd < 0) {
        perror("open /proc/self/gid_map");
        exit(EXIT_FAILURE);
    }

    char gid_map[255];
    sprintf(gid_map, "0 %d 1", (int)gid);

    ret = write(fd, gid_map, sizeof(gid_map));
    if (ret != sizeof(gid_map)) {
        perror("write /proc/self/gid_map");
        exit(EXIT_FAILURE);
    }
    close(fd);
}

int main(void) {
    const uid_t host_uid = getuid();
    const gid_t host_gid = getgid();

    // PID, UTS, Network, IPC, User, Mountを分離する
    const int clone_flags = CLONE_NEWPID | CLONE_NEWUTS | CLONE_NEWNET |
                            CLONE_NEWIPC | CLONE_NEWUSER | CLONE_NEWNS;

    // Hostと別のLinux Namespaceを作成する
    int err = unshare(clone_flags);
    if (err < 0) {
        perror("unshare");
        exit(EXIT_FAILURE);
    }

    // HostのUIDをコンテナのUID 0に割り当てる
    new_uidmap(host_uid);
    // HostのGIDをコンテナのGID 0に割り当てる
    new_gidmap(host_gid);

    // 子プロセスの作成
    const pid_t pid = fork();

    if (pid < 0) {
        perror("fork");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        // Child Process
        int ret = chroot("rootfs");
        if (ret < 0) {
            perror("chroot");
            exit(EXIT_FAILURE);
        }
        chdir("/");

        ret = mount("proc", "/proc", "proc", 0, NULL);
        if (ret < 0) {
            perror("mount procfs");
            exit(EXIT_FAILURE);
        }

        char *const child_argv[2] = {"/bin/bash", NULL};
        char *const env[] = {};
        execve(child_argv[0], child_argv, env);
        perror("execve");
        exit(EXIT_FAILURE);
    }

    // Parent Process
    int status;

    const pid_t ret = waitpid(pid, &status, 0);

    if (ret < 0) {
        perror("waitpid");
        exit(EXIT_FAILURE);
    }

    // プロセスが終了している場合
    if (WIFEXITED(status)) {
        printf("child exit code: %d\n", WEXITSTATUS(status));
    } else {
        printf("child status: %04d\n", status);
    }
}
