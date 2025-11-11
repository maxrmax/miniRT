#include "miniRT.h"

int validate_scene(t_rt *scene)
{
    if (!scene)
    {
        printf("Error: scene is NULL\n");
        return 0;
    }

    // Check ambient light
    if (!scene->ambient)
    {
        printf("Error: missing ambient light\n");
        return 0;
    }
    if (scene->ambient->ratio < 0.0f || scene->ambient->ratio > 1.0f)
    {
        printf("Error: ambient ratio out of range (0.0-1.0)\n");
        return 0;
    }

    // Check camera
    if (!scene->camera)
    {
        printf("Error: missing camera\n");
        return 0;
    }
    // Check camera direction is not zero
    if (scene->camera->dir.x == 0 && scene->camera->dir.y == 0 && scene->camera->dir.z == 0)
    {
        printf("Error: camera direction cannot be zero vector\n");
        return 0;
    }
    if (scene->camera->fov < 0 || scene->camera->fov > 180)
    {
        printf("Error: camera FOV out of range (0-180)\n");
        return 0;
    }

    // Check light
    if (!scene->light)
    {
        printf("Error: missing light\n");
        return 0;
    }
    if (scene->light->brightness < 0.0f || scene->light->brightness > 1.0f)
    {
        printf("Error: light brightness out of range (0.0-1.0)\n");
        return 0;
    }

    // Check objects if allocated
    if (scene->sphere)
    {
        if (scene->sphere->diameter <= 0)
        {
            printf("Error: sphere diameter must be > 0\n");
            return 0;
        }
    }

    if (scene->palne)
    {
        if (scene->palne->normal.x == 0 && scene->palne->normal.y == 0 && scene->palne->normal.z == 0)
        {
            printf("Error: plane normal cannot be zero vector\n");
            return 0;
        }
    }

    if (scene->cylinder)
    {
        if (scene->cylinder->diameter <= 0 || scene->cylinder->hight <= 0)
        {
            printf("Error: cylinder size must be > 0\n");
            return 0;
        }
    }

    // If we reach here, everything looks valid
    return 1;
}
