/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_scene.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mring <mring@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/15 15:45:28 by mring             #+#    #+#             */
/*   Updated: 2025/11/20 16:30:19 by mring            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

void	draw_elements(t_rt *scenes)
{
	// ambient has no pos, its deciding color and ratio
	// think about its implementation
	if (scenes->camera)
		vec_new(scenes->camera->pos.x, scenes->camera->pos.y,
			scenes->camera->pos.z);
	if (scenes->light)
		vec_new(scenes->light->pos.x, scenes->light->pos.y,
			scenes->light->pos.z);
	// am i missing anything?
	// matrixes?
	// TODO: research
}

void	object_drawing(t_rt *scenes)
{
	int	i;

	draw_elements(scenes);
	i = 0;
	// draw objects
	while (i < scenes->count)
	{
		// sphere has vec3 center, float diameter,
		// how to place it?
		// if (scenes->sphere)              //[i])
		// draw_sphere(scenes->sphere); //[i]);
		// plane has a vec3 point, a normal, and color
		// if (scenes->plane[i])
		// draw_plane(scenes->plane[i]);
		// cylinder has center, axis (matrix?), diameter, height, color
		// if (scenes->cylinder[i])
		// draw_cylinder(scenes->cylinder[i]);
		i++;
	}
	// same here, anything missing?
	// TODO: learn how to apply matrix.
	// TODO: research
}

int	draw_scene(t_rt *scenes)
{
	printf("draw_scene\n");
	return (-1);
	object_drawing(scenes);
	// draw every object in the list
	// will later be an array needed to be iterated.
	// can be multiple cy, pl, sp
	// then apply matrix: rotation, color, etc.
	return (0);
}

#include "miniRT.h"
#include <math.h>

// Ray structure
typedef struct s_ray
{
	t_vec3	origin;
	t_vec3	direction;
}			t_ray;

// Get point along ray: origin + t * direction
t_vec3	ray_at(t_ray *ray, float t)
{
	t_vec3	scaled;

	scaled = vec_mult(ray->direction, t);
	return (vec_add(ray->origin, scaled));
}

// Check if ray hits a sphere, return distance (or -1 if no hit)
float	hit_sphere(t_vec3 center, float radius, t_ray *ray)
{
	t_vec3	oc;
	float	a;
	float	b;
	float	c;
	float	discriminant;

	// Vector from ray origin to sphere center
	oc = vec_sub(ray->origin, center);
	// Quadratic equation: at² + bt + c = 0
	a = vec_dot(ray->direction, ray->direction);
	b = 2.0 * vec_dot(oc, ray->direction);
	c = vec_dot(oc, oc) - radius * radius;
	discriminant = b * b - 4 * a * c;
	// No intersection
	if (discriminant < 0)
		return (-1.0);
	// Return closest hit point
	return ((-b - sqrt(discriminant)) / (2.0 * a));
}

// Simple lighting calculation
t_vec3	calculate_color(t_ray *ray, float t, t_vec3 sphere_center, t_rt *scene)
{
	t_vec3	hit_point;
	t_vec3	normal;
	t_vec3	light_dir;
	float	brightness;
	t_vec3	color;
	float	ambient;

	// Point where ray hit sphere
	hit_point = ray_at(ray, t);
	// Normal at hit point (pointing outward from sphere)
	normal = vec_sub(hit_point, sphere_center);
	normal = vec_normalize(normal);
	// Direction from hit point to light
	light_dir = vec_sub(scene->light->pos, hit_point);
	light_dir = vec_normalize(light_dir);
	// Simple diffuse lighting (dot product of normal and light direction)
	brightness = vec_dot(normal, light_dir);
	if (brightness < 0)
		brightness = 0;
	// Combine sphere color with lighting
	color.x = scene->sphere->color.r / 255.0 * brightness;
	color.y = scene->sphere->color.g / 255.0 * brightness;
	color.z = scene->sphere->color.b / 255.0 * brightness;
	// Add ambient light
	ambient = scene->ambient->ratio;
	color.x += ambient * scene->ambient->color.r / 255.0;
	color.y += ambient * scene->ambient->color.g / 255.0;
	color.z += ambient * scene->ambient->color.b / 255.0;
	// Clamp to 0-1 range
	if (color.x > 1.0)
		color.x = 1.0;
	if (color.y > 1.0)
		color.y = 1.0;
	if (color.z > 1.0)
		color.z = 1.0;
	return (color);
}

// ============================================================================
// SIMPLE_RENDER: Complete ray tracing pipeline from parsed scene to pixels
// ============================================================================
// Purpose: This is the MAIN RENDERING FUNCTION that converts your parsed
//          scene data into actual pixels on screen using ray tracing.
//
// What it does step-by-step:
// 1. Creates MLX window and image buffer
// 2. Sets up virtual camera and viewport in 3D space
// 3. For EACH pixel: shoots a ray into the scene
// 4. Tests if ray hits any objects (sphere in this case)
// 5. Calculates color based on lighting
// 6. Writes color to image buffer
// 7. Displays the final rendered image
// ============================================================================
int	simple_render(t_rt *scene)
{
	t_vec3	horizontal;

	// ========================================================================
	// VARIABLE DECLARATIONS - ALL AT TOP (C89/C90 requirement)
	// ========================================================================
	// --- MLX Window & Image ---
	mlx_t *window;    // MLX window handle (the actual OS window)
	mlx_image_t *img; // Image buffer (RGBA pixel data array)
	// --- Screen Dimensions ---
	int width;  // Window width in pixels (1280 from miniRT.h)
	int height; // Window height in pixels (720 from miniRT.h)
	// --- Camera Setup ---
	t_vec3 camera_pos; // Camera position in 3D world space
	// --- Virtual Viewport (the "screen" in 3D space) ---
	float viewport_height; // Height of viewport in world units (2.0)
	float viewport_width;  // Width of viewport (calculated from aspect ratio)
	float focal_length;    // Distance from camera to viewport (1.0 unit)
	// --- Viewport Basis Vectors ---
	// Vector spanning full width of viewport (right direction)
	t_vec3 vertical; // Vector spanning full height of viewport (up direction)
	t_vec3 focal;    // UNUSED in final code (leftover from simple setup)
	// --- Viewport Corner Calculation ---
	t_vec3 half_h;     // Half of horizontal vector (for centering)
	t_vec3 half_v;     // Half of vertical vector (for centering)
	t_vec3 lower_left; // Bottom-left corner of viewport in 3D space
	// --- Per-Pixel UV Coordinates ---
	float u; // Horizontal position across viewport (0.0 to 1.0)
	float v; // Vertical position across viewport (0.0 to 1.0)
	// --- Offset Vectors for Current Pixel ---
	t_vec3 offset_h; // How far to move right from lower_left (horizontal)
	t_vec3 offset_v; // How far to move up from lower_left (vertical)
	// --- Ray Tracing ---
	t_ray ray; // The ray we shoot from camera through current pixel
	float t;   // Distance along ray where intersection occurs
	// --- Color Calculation ---
	t_vec3 pixel_color; // Final calculated color (RGB as floats 0.0-1.0)
	int r;              // Red component (0-255 for image buffer)
	int g;              // Green component (0-255)
	int b;              // Blue component (0-255)
	// --- Background (unused in final version) ---
	float bg; // UNUSED: was for gradient calculation
	int gray; // UNUSED: was for gradient value
	// --- Camera Orientation ---
	t_vec3 look_at;  // Point in space camera is looking at (sphere center)
	t_vec3 view_dir; // Normalized direction vector camera is facing
	t_vec3 world_up; // World's "up" direction (0, 1, 0 = Y-axis up)
	t_vec3 right;    // Camera's "right" direction (local X-axis)
	t_vec3 up;       // Camera's "up" direction (local Y-axis)
	// --- Loop Counters ---
	int y; // Current row (pixel Y coordinate)
	int x; // Current column (pixel X coordinate)
	// --- Image Buffer Index ---
	uint32_t index; // Byte offset into img->pixels array
	// ========================================================================
	// STEP 1: WINDOW & IMAGE INITIALIZATION
	// ========================================================================
	// Create the actual OS window where we'll display our rendered image
	width = WIDTH;   // Get dimensions from miniRT.h (1280)
	height = HEIGHT; // Get dimensions from miniRT.h (720)
	window = mlx_init(width, height, "miniRT", false);
	if (!window)
		return (-1); // Failed to create window
	// Create image buffer (array of RGBA pixels)
	img = mlx_new_image(window, width, height);
	if (!img)
	{
		mlx_terminate(window); // Clean up window if image creation fails
		return (-1);
	}
	// ========================================================================
	// STEP 2: CAMERA SETUP
	// ========================================================================
	// Get camera position from parsed scene file
	camera_pos = scene->camera->pos; // e.g., (-50, 0, 20) from mandatory.rt
	// ========================================================================
	// STEP 3: VIEWPORT SETUP
	// ========================================================================
	// The viewport is like a "window" in 3D space that we look through.
	// Think of it as a physical screen floating in front of the camera.
	viewport_height = 2.0; // Arbitrary unit (2.0 works well for most scenes)
							// Smaller = more zoomed in,
	// Larger = more zoomed out viewport_width = viewport_height * ((float)width
	// / height);
	// Why calculate this way?
	// - We want viewport aspect ratio to match window aspect ratio
	// - If window is 1280x720 = 16:9 ratio
	// - Then viewport should also be 16:9 to avoid distortion
	// - viewport_width = 2.0 * (1280/720) ≈ 3.56
	focal_length = 1.0; // Distance from camera to viewport
						// Controls field of view (FOV):
						// - Smaller focal_length = wider FOV (see more)
						// - Larger focal_length = narrower FOV (zoomed in)
	// ========================================================================
	// STEP 4: CAMERA ORIENTATION (Look-At System)
	// ========================================================================
	// We need to orient the camera to look at something.
	// For this simple version, we just point it at the sphere.
	look_at = scene->sphere->center; // Target point (where sphere is)
	// Calculate view direction: from camera toward target
	view_dir = vec_sub(look_at, camera_pos); // Vector from camera to sphere
	view_dir = vec_normalize(view_dir);      // Make it unit length
	// Example: camera at (-50,0,20), sphere at (0,0,20)
	// view_dir = (0-(-50), 0-0, 20-20) = (50, 0, 0)
	// normalized = (1, 0, 0) pointing along +X axis
	// ========================================================================
	// STEP 5: BUILD CAMERA COORDINATE SYSTEM
	// ========================================================================
	// We need 3 perpendicular vectors to define camera's orientation:
	// - view_dir: forward (where camera looks)
	// - right: camera's right direction
	// - up: camera's up direction
	world_up = vec_new(0, 1, 0); // World's up is Y-axis
									// This is our reference "up" direction
	// Calculate RIGHT vector using cross product
	right = vec_cross(view_dir, world_up);
	// Cross product gives perpendicular vector!
	// view_dir × world_up = right direction
	// Example: (1,0,0) × (0,1,0) = (0,0,1) pointing along +Z
	right = vec_normalize(right);
	// Calculate UP vector using another cross product
	up = vec_cross(right, view_dir);
	// right × view_dir = up direction
	// This ensures up is perpendicular to BOTH right and view_dir
	// We don't just use world_up because camera might be tilted!
	// Now we have an orthonormal basis (3 perpendicular unit vectors)
	// that define camera's local coordinate system
	// ========================================================================
	// STEP 6: CREATE VIEWPORT VECTORS
	// ========================================================================
	// Scale the right/up vectors to span the viewport dimensions
	horizontal = vec_mult(right, viewport_width);
	// This vector spans the entire width of the viewport
	// Length = viewport_width (e.g., 3.56 units)
	// Direction = camera's right direction
	vertical = vec_mult(up, viewport_height);
	// This vector spans the entire height of the viewport
	// Length = viewport_height (2.0 units)
	// Direction = camera's up direction
	// ========================================================================
	// STEP 7: CALCULATE VIEWPORT LOWER-LEFT CORNER
	// ========================================================================
	// We need to know where the viewport starts in 3D space.
	// We'll use the lower-left corner as our reference point.
	half_h = vec_mult(horizontal, 0.5); // Half of horizontal vector
	half_v = vec_mult(vertical, 0.5);   // Half of vertical vector
	// Why halves? To center the viewport around the view direction!
	lower_left = vec_add(camera_pos, view_dir); // Start at camera + forward
	lower_left = vec_sub(lower_left, half_h);   // Move left by half width
	lower_left = vec_sub(lower_left, half_v);   // Move down by half height
	// Now lower_left points to the bottom-left corner of our viewport!
	// ========================================================================
	// STEP 8: MAIN PIXEL LOOP - THE RAYTRACING HAPPENS HERE!
	// ========================================================================
	// For every single pixel on screen, we:
	// 1. Calculate a ray direction from camera through that pixel
	// 2. Test if ray hits any objects
	// 3. Calculate color based on hit point and lighting
	// 4. Write color to image buffer
	y = 0;
	while (y < height) // For each row of pixels
	{
		x = 0;
		while (x < width) // For each column of pixels
		{
			// ================================================================
			// STEP 8.1: CALCULATE UV COORDINATES FOR CURRENT PIXEL
			// ================================================================
			// Convert pixel coordinates (x, y) to normalized UV coords (0-1)
			u = (float)x / (width - 1);
			// u goes from 0.0 (left edge) to 1.0 (right edge)
			// Example: x=0 → u=0.0, x=1279 → u=1.0 (for 1280 pixels wide)
			v = (float)(height - 1 - y) / (height - 1);
			// v goes from 0.0 (bottom) to 1.0 (top)
			// Why flip Y? Screen coords are top-down,
			// but viewport is bottom-up!
			// Example: y=0 (top) → v=1.0, y=719 (bottom) → v=0.0
			// ================================================================
			// STEP 8.2: CALCULATE OFFSETS FROM LOWER-LEFT CORNER
			// ================================================================
			offset_h = vec_mult(horizontal, u);
			// How far to move right from lower_left
			offset_v = vec_mult(vertical, v);
			// How far to move up from lower_left
			// ================================================================
			// STEP 8.3: BUILD THE RAY FOR THIS PIXEL
			// ================================================================
			ray.origin = camera_pos; // Every ray starts at camera
			ray.direction = vec_add(lower_left, offset_h);
			ray.direction = vec_add(ray.direction, offset_v);
			ray.direction = vec_sub(ray.direction, camera_pos);
			ray.direction = vec_normalize(ray.direction);
			// Now we have a ray from camera through this pixel into the scene
			// ================================================================
			// STEP 8.4: RAY-SPHERE INTERSECTION TEST
			// ================================================================
			t = hit_sphere(scene->sphere->center, scene->sphere->diameter / 2.0,
					&ray);
			// Returns distance along ray where hit occurs
			// t < 0 = no hit, t > 0 = hit!
			if (t > 0.0) // Ray hit the sphere!
			{
				// ============================================================
				// STEP 8.5: CALCULATE COLOR FOR HIT POINT
				// ============================================================
				pixel_color = calculate_color(&ray, t, scene->sphere->center,
						scene);
				r = (int)(pixel_color.x * 255); // Convert 0.0-1.0 → 0-255
				g = (int)(pixel_color.y * 255);
				b = (int)(pixel_color.z * 255);
				// ============================================================
				// STEP 8.6: WRITE PIXEL TO IMAGE BUFFER
				// ============================================================
				index = (y * width + x) * 4;  // RGBA = 4 bytes per pixel
				img->pixels[index + 0] = r;   // Red
				img->pixels[index + 1] = g;   // Green
				img->pixels[index + 2] = b;   // Blue
				img->pixels[index + 3] = 255; // Alpha (opaque)
			}
			else // Ray missed the sphere
			{
				// ============================================================
				// STEP 8.7: BACKGROUND COLOR
				// ============================================================
				index = (y * width + x) * 4;
				img->pixels[index + 0] = 50; // Dark blue background
				img->pixels[index + 1] = 100;
				img->pixels[index + 2] = 150;
				img->pixels[index + 3] = 255;
			}
			x++; // Move to next pixel in row
		}
		y++; // Move to next row
	}
	// ========================================================================
	// STEP 9: DISPLAY THE RENDERED IMAGE
	// ========================================================================
	mlx_image_to_window(window, img, 0, 0);
	// Put the image into the window at position (0, 0)
	// ========================================================================
	// STEP 10: EVENT LOOP
	// ========================================================================
	mlx_loop(window);
	// Keep window open, blocks until user closes window
	// ========================================================================
	// STEP 11: CLEANUP
	// ========================================================================
	mlx_delete_image(window, img);
	mlx_terminate(window);
	return (0);
}

int	simple_render1(t_rt *scene)
{
	mlx_t		*window;
	mlx_image_t	*img;
	int			width;
	int			height;
	t_vec3		camera_pos;
	float		viewport_height;
	float		viewport_width;
	float		focal_length;
	t_vec3		horizontal;
	t_vec3		vertical;
	t_vec3		focal;
	t_vec3		half_h;
	t_vec3		half_v;
	t_vec3		lower_left;
	float		u;
	float		v;
	t_vec3		offset_h;
	t_vec3		offset_v;
	t_ray		ray;
	float		t;
	t_vec3		pixel_color;
	int			r;
	int			g;
	int			b;
	float		bg;
	int			gray;
	t_vec3		look_at;
	t_vec3		view_dir;
	t_vec3		world_up;
	t_vec3		right;
	t_vec3		up;
	int			y;
	int			x;

	uint32_t index; // Remove duplicate declaration
	// Initialize window
	width = WIDTH;
	height = HEIGHT;
	window = mlx_init(width, height, "miniRT", false);
	if (!window)
		return (-1);
	img = mlx_new_image(window, width, height);
	if (!img)
	{
		mlx_terminate(window);
		return (-1);
	}
	camera_pos = scene->camera->pos;
	viewport_height = 2.0;
	viewport_width = viewport_height * ((float)width / height);
	focal_length = 1.0;
	// Calculate direction from camera to sphere center
	look_at = scene->sphere->center;
	view_dir = vec_sub(look_at, camera_pos);
	view_dir = vec_normalize(view_dir);
	// For now: simple right/up vectors (will need proper camera orientation later)
	world_up = vec_new(0, 1, 0);
	right = vec_cross(view_dir, world_up);
	right = vec_normalize(right);
	up = vec_cross(right, view_dir);
	horizontal = vec_mult(right, viewport_width);
	vertical = vec_mult(up, viewport_height);
	// Lower left corner
	half_h = vec_mult(horizontal, 0.5);
	half_v = vec_mult(vertical, 0.5);
	lower_left = vec_add(camera_pos, view_dir); // Start at camera + forward
	lower_left = vec_sub(lower_left, half_h);
	lower_left = vec_sub(lower_left, half_v);
	y = 0;
	while (y < height)
	{
		x = 0;
		while (x < width)
		{
			u = (float)x / (width - 1);
			v = (float)(height - 1 - y) / (height - 1);
			offset_h = vec_mult(horizontal, u);
			offset_v = vec_mult(vertical, v);
			ray.origin = camera_pos;
			ray.direction = vec_add(lower_left, offset_h);
			ray.direction = vec_add(ray.direction, offset_v);
			ray.direction = vec_sub(ray.direction, camera_pos);
			ray.direction = vec_normalize(ray.direction);
			// Use 'center' instead of 'pos' for sphere
			t = hit_sphere(scene->sphere->center, scene->sphere->diameter / 2.0,
					&ray);
			if (t > 0.0)
			{
				pixel_color = calculate_color(&ray, t, scene->sphere->center,
						scene);
				r = (int)(pixel_color.x * 255);
				g = (int)(pixel_color.y * 255);
				b = (int)(pixel_color.z * 255);
				index = (y * width + x) * 4;
				img->pixels[index + 0] = r;
				img->pixels[index + 1] = g;
				img->pixels[index + 2] = b;
				img->pixels[index + 3] = 255;
			}
			else
			{
				index = (y * width + x) * 4;
				img->pixels[index + 0] = 50; // Black
				img->pixels[index + 1] = 100;
				img->pixels[index + 2] = 150;
				img->pixels[index + 3] = 255;
			}
			x++;
		}
		y++;
	}
	mlx_image_to_window(window, img, 0, 0);
	mlx_loop(window);
	mlx_delete_image(window, img);
	mlx_terminate(window);
	return (0);
}
