# miniRT Program Flow - Complete Walkthrough

This document explains exactly what happens when you run `./miniRT scene.rt`, line by line. Think of it as following a path through the code from start to finish.

---

## What is Ray Tracing?

Imagine you're taking a photo. Your eye (camera) shoots invisible lines (rays) through every pixel of the photo frame, and wherever those lines hit an object, you see that object's color. That's ray tracing - we're simulating how light travels backwards from the camera to objects.

---

## Starting Point: main()

**File:** `src/main.c`

```c
int main(int ac, char **av)
```

### Line 1: Check arguments
```c
if (ac != 2)
```
- We need exactly 2 arguments: the program name and the scene file
- Example: `./miniRT scenes/mandatory.rt`
- If wrong, print error and exit

### Line 2: Allocate scene memory
```c
scene = ft_calloc(1, sizeof(t_rt));
```
- `ft_calloc` allocates memory and sets everything to 0
- `scene` is our main data container - it holds EVERYTHING:
  - Camera position and direction
  - Light position and brightness
  - All objects (spheres, planes, cylinders)
  - The window and image data
- Why calloc instead of malloc? Setting to 0 means all pointers start as NULL (safe)

### Line 3: Parse the scene file
```c
if (parsing_scene(av[1], scene))
    return (free_scenes(scene), 1);
```
- Opens the `.rt` file and reads line by line
- Creates objects from lines like `sp 0,0,20 10 255,0,0` (sphere)
- Stores everything in `scene`
- If parsing fails (bad file format), clean up and exit

### Line 4: Validate required elements
```c
if (validate_scene(scene))
```
- Checks that we have:
  - One ambient light (A)
  - One camera (C)
  - One light source (L)
- Without these three, we can't render anything

### Line 5: Start rendering
```c
window_loop(scene);
```
- This is where the magic happens - opens window and renders the image

### Line 6: Clean up and exit
```c
free_scenes(scene);
return (0);
```
- Frees all allocated memory (objects, camera, light, scene)
- Program ends cleanly

---

## The Rendering Engine: window_loop()

**File:** `src/window_loop.c`

This function sets up the window and renders the entire image.

```c
void window_loop(t_rt *scene)
```

### Step 1: Initialize tracking variables
```c
scene->hit_obj = NULL;
scene->width = WIDTH;
scene->height = HEIGHT;
```
- `hit_obj` - will store which object a ray hit (starts as NULL = nothing)
- `width`, `height` - window dimensions (e.g., 800x600)

### Step 2: Create the window
```c
init_window(scene);
```
Calls a helper function that does:
```c
scene->window = mlx_init(scene->width, scene->height, "miniRT", true);
```
- Creates a window using MLX42 library
- Dimensions from WIDTH and HEIGHT defines
- Title: "miniRT"
- `true` = window is resizable

```c
scene->img = mlx_new_image(scene->window, scene->width, scene->height);
```
- Creates an image buffer (pixel array) inside the window
- This is where we'll draw our rendered scene
- Image has `width * height * 4` bytes (RGBA: Red, Green, Blue, Alpha)

### Step 3: Pre-calculate camera setup
```c
pre_calc_camera(scene);
```
**File:** `src/logic/camera_calc.c`

This calculates values we'll reuse for every pixel.

#### Line 1: Convert FOV to radians
```c
scene->camera->fov_rad = scene->camera->fov * M_PI / 180.0;
```
- FOV (Field of View) comes from scene file in degrees (e.g., 70)
- Math functions need radians, so we convert
- Example: 70° × π/180 = 1.22 radians

#### Line 2: Calculate viewport height
```c
scene->camera->viewport_height = 2.0 * tan(scene->camera->fov_rad / 2.0);
```
- **Viewport** = the virtual screen we're projecting through
- Imagine a rectangle floating in front of the camera
- `tan(fov/2)` gives us the "spread" of the viewing angle
- Why × 2? We need both sides (up and down from center)

**Visual:**
```
         /|
camera  / | viewport_height/2
       /  |
      /__θ|  (θ = fov/2)
     /    |
```

#### Line 3: Calculate viewport width (aspect ratio)
```c
scene->camera->viewport_width = scene->camera->viewport_height
    * ((double)scene->img->width / scene->img->height);
```
- Width depends on our window's aspect ratio
- If window is 800×600 (4:3), viewport is also 4:3
- Prevents stretching - a sphere stays round

#### Line 4-7: Build camera coordinate system
```c
scene->camera->forward = vec_normalize(scene->camera->dir);
```
- `forward` = where the camera looks (normalized to length 1)

```c
scene->camera->world_up = vec_new(0, 1, 0);
```
- This is "up" in the world (positive Y axis)

```c
scene->camera->right = vec_normalize(vec_cross(scene->camera->forward,
    scene->camera->world_up));
```
- **Cross product** creates a vector perpendicular to both inputs
- `forward` × `world_up` = `right` (pointing to camera's right side)
- Think right-hand rule: point forward with index, up with middle, thumb shows right

```c
scene->camera->up = vec_cross(scene->camera->right, scene->camera->forward);
```
- Now we have all three axes for the camera:
  - `forward` - where it looks
  - `right` - camera's horizontal axis
  - `up` - camera's vertical axis

**Why?** We need these to calculate ray directions for each pixel.

### Step 4: Render the scene
```c
render_scene(scene);
```

#### The pixel loops
```c
i = 0;
while (i < scene->img->height)     // For each row (top to bottom)
{
    while (j < scene->img->width)  // For each column (left to right)
    {
        render_pixel(scene, i, j);
        j++;
    }
    j = 0;
    i++;
}
```
- Nested loops iterate through every single pixel
- Example: 800×600 = 480,000 iterations
- For each pixel at position (j, i), we call `render_pixel`

---

## Rendering One Pixel: render_pixel()

**File:** `src/window_loop.c`

```c
static void render_pixel(t_rt *scene, int i, int j)
{
    calc_camera(scene, i, j);   // Step 1: Create ray
    calc_objs(scene);            // Step 2: Find what ray hits
    calc_pixel(scene);           // Step 3: Calculate color
}
```

Three simple steps for each pixel. Let's break them down.

---

## Step 1: Create the Ray - calc_camera()

**File:** `src/logic/camera_calc.c`

```c
void calc_camera(t_rt *scene, int i, int j)
```

We need to shoot a ray from the camera through pixel (j, i) into the world.

### Calculate viewport position (pixel to viewport coordinates)

#### Horizontal position
```c
viewport_x = ((double)j / (scene->img->width - 1) * 2.0 - 1.0)
    * (scene->camera->viewport_width / 2.0);
```

Let's break this down step by step:

1. `j / (width - 1)` - convert pixel column to range [0, 1]
   - Example: pixel 400 of 800 → 400/799 = 0.5 (middle)
   
2. `* 2.0 - 1.0` - convert [0, 1] to [-1, 1]
   - 0 becomes -1 (left edge)
   - 0.5 becomes 0 (center)
   - 1 becomes 1 (right edge)
   
3. `* (viewport_width / 2.0)` - scale to actual viewport size
   - Now we have the actual distance from center

#### Vertical position
```c
viewport_y = (1.0 - (double)i / (scene->img->height - 1) * 2.0)
    * (scene->camera->viewport_height / 2.0);
```
- Same logic, but `1.0 -` flips it (screen Y goes down, world Y goes up)
- Row 0 (top) → viewport_y is positive (up)
- Row 599 (bottom) → viewport_y is negative (down)

### Build the ray direction

#### Combine right and up vectors
```c
viewport_offset = vec_add(vec_mult(scene->camera->right, viewport_x),
                          vec_mult(scene->camera->up, viewport_y));
```

**What's happening:**
1. `vec_mult(right, viewport_x)` - move right by viewport_x amount
2. `vec_mult(up, viewport_y)` - move up by viewport_y amount
3. `vec_add` - combine them into one offset vector

**Visual:**
```
        up
        |
        |___right
       /
   forward
```
We're offsetting from the forward direction.

#### Create final ray direction
```c
scene->objects->ray_dir = vec_normalize(vec_add(scene->camera->forward,
                                                viewport_offset));
```
- `forward + offset` = ray pointing through this pixel
- `vec_normalize` = make length 1 (direction only, not distance)
- Stored in `objects->ray_dir` (will be copied to each object)

### Calculate pixel index in image buffer
```c
scene->index = (i * scene->img->width + j) * 4;
```
- Image is 1D array: `[R, G, B, A, R, G, B, A, ...]`
- Row `i`, column `j` → position `(i * width + j)` in pixel grid
- `* 4` because each pixel has 4 bytes (RGBA)
- Example: pixel at (10, 5) in 800-wide image → (5 * 800 + 10) * 4 = 16,040

**Now we have a ray shooting from camera through this pixel!**

---

## Step 2: Find What the Ray Hits - calc_objs()

**File:** `src/logic/obj_calc.c`

```c
void calc_objs(t_rt *scene)
```

We test the ray against every object in the scene to find the closest hit.

### Setup
```c
obj = scene->objects;
ray_origin = scene->camera->pos;
closest_t = INFINITY;
scene->hit_obj = NULL;
```
- `obj` - pointer to first object in linked list
- `ray_origin` - ray starts at camera position
- `closest_t` - distance to closest object (starts as infinity)
- `hit_obj` - which object we hit (starts as NULL)

### The object loop
```c
while (obj)
{
    obj->ray_origin = ray_origin;
    obj->ray_dir = scene->objects->ray_dir;
    obj->t = -1.0;
```
- Loop through every object
- Give each object the ray information
- `t` = distance along ray to intersection (-1 means no hit)

### Test intersection based on object type
```c
if (obj->type == SPHERE)
    obj->t = hit_sphere(obj);
else if (obj->type == PLANE)
    obj->t = hit_plane(obj);
else if (obj->type == CYLINDER)
    obj->t = hit_cylinder(obj);
```
- Call different math functions for different shapes
- Each returns distance `t` (or -1 if no hit)

### Keep track of closest hit
```c
closest_t = epsilon(scene, obj, closest_t);
```

The `epsilon` helper:
```c
if (obj->t > 0 && obj->t < closest_t)
{
    scene->hit_obj = obj;
    return (obj->t);
}
return (closest_t);
```
- If this hit is closer than previous closest, update it
- We want the FIRST object the ray hits (closest to camera)

### Store final result
```c
if (scene->hit_obj)
    scene->hit_obj->closest_t = closest_t;
```
- Save the distance for later lighting calculations

**Now we know which object (if any) the ray hit first!**

---

## Intersection Math: How Do We Know If Ray Hits Object?

### Ray Equation
A ray is defined by:
```
point = origin + direction * t
```
- `origin` - where ray starts (camera position)
- `direction` - which way it goes (normalized vector)
- `t` - how far along the ray (distance)

When `t = 0`, we're at origin. When `t = 5`, we're 5 units along the ray.

---

### Sphere Intersection: hit_sphere()

**File:** `src/logic/sphere.c`

```c
double hit_sphere(t_obj *obj)
```

A sphere is all points at distance `radius` from `center`:
```
(point - center)² = radius²
```

Substitute ray equation:
```
(origin + dir*t - center)² = radius²
```

This becomes a **quadratic equation**: `at² + bt + c = 0`

#### Line 1-2: Calculate coefficients
```c
a = vec_dot(obj->ray_dir, obj->ray_dir);
```
- Dot product of direction with itself = length squared
- Since `ray_dir` is normalized (length 1), `a = 1`

```c
b = 2.0 * vec_dot(obj->ray_dir, vec_sub(obj->ray_origin,
                                        obj->data.sp.center));
```
- This relates ray direction to the vector from sphere center to ray origin
- The `2.0 *` comes from expanding the quadratic formula

```c
c = vec_dot(vec_sub(obj->ray_origin, obj->data.sp.center),
           vec_sub(obj->ray_origin, obj->data.sp.center))
    - obj->data.sp.diameter * obj->data.sp.diameter / 4;
```
- Distance squared from ray origin to sphere center, minus radius squared
- `diameter/2 = radius`, so `radius² = diameter²/4`

#### Line 3: Calculate discriminant
```c
discriminant = b * b - 4.0 * a * c;
```
- From quadratic formula: `(-b ± √discriminant) / 2a`
- Discriminant tells us:
  - `< 0` → no real solutions → ray misses sphere
  - `= 0` → one solution → ray grazes sphere
  - `> 0` → two solutions → ray enters and exits sphere

#### Line 4-5: Check if ray misses
```c
if (discriminant < 0)
    return (-1.0);
```
- No intersection, return -1

#### Line 6: Find closest intersection
```c
obj->t = (-b - sqrt(discriminant)) / (2.0 * a);
```
- Quadratic formula gives us `t` (distance along ray)
- We use `-` (not `+`) because we want the FIRST hit (entry point)
- The `+` would give us the exit point (back of sphere)

#### Line 7-8: Check if intersection is behind camera
```c
if (obj->t < 0)
    return (-1.0);
return (obj->t);
```
- If `t < 0`, intersection is behind camera (we don't see it)
- Otherwise, return the distance

---

### Plane Intersection: hit_plane()

**File:** `src/logic/plane.c`

```c
double hit_plane(t_obj *obj)
```

A plane is defined by:
- A point on the plane
- A normal vector (perpendicular to plane surface)

#### Line 1: Check if ray is parallel to plane
```c
denom = vec_dot(obj->ray_dir, obj->data.pl.normal);
if (fabs(denom) < 1e-6)
    return (-1.0);
```
- Dot product of ray direction and plane normal
- If this is ~0, ray is parallel to plane (never intersects)
- `1e-6` = 0.000001 (small epsilon for floating-point comparison)

**Why dot product?**
- If ray points same direction as normal: dot > 0 (hitting front)
- If perpendicular: dot = 0 (parallel, no hit)
- If opposite direction: dot < 0 (hitting back)

#### Line 2: Calculate distance to intersection
```c
obj->t = vec_dot(vec_sub(obj->data.pl.point, obj->ray_origin),
                 obj->data.pl.normal) / denom;
```
- `plane_point - ray_origin` = vector from ray to plane
- Dot with normal = perpendicular distance
- Divide by `denom` to get `t` along ray direction

#### Line 3: Check if behind camera
```c
if (obj->t < 0)
    return (-1.0);
return (obj->t);
```

---

### Cylinder Intersection: hit_cylinder()

**File:** `src/logic/cylinder_main.c` and `src/logic/cylinder_calc.c`

Cylinders are the most complex! They have:
1. A curved body (infinite cylinder)
2. Top and bottom caps (circles)

```c
double hit_cylinder(t_obj *obj)
{
    if (cy_base(obj) < 0)     // Test infinite cylinder body
        return (-1.0);
    cy_height(obj);            // Check if hit is within height bounds
    cap_calc(obj);             // Test top and bottom caps
    return (obj->t);
}
```

#### Part 1: Cylinder Body - cy_base()

The body is like a 2D circle problem in 3D space. We:
1. Project everything perpendicular to the cylinder's axis
2. Solve 2D circle intersection
3. Extend back to 3D

**Step 1: Get origin-to-center vector**
```c
objcent = vec_sub(obj->ray_origin, obj->data.cy.center);
```
- Vector from cylinder center to ray origin

**Step 2: Project ray perpendicular to axis**
```c
ray_perp = calc_ray_perp(obj);
```

Inside `calc_ray_perp`:
```c
dot = vec_dot(obj->ray_dir, obj->data.cy.axis);
projection = vec_mult(obj->data.cy.axis, dot);
return (vec_sub(obj->ray_dir, projection));
```
- `dot` = how much ray points along axis
- `projection` = component of ray along axis
- `ray - projection` = perpendicular component

**Visual:**
```
     axis
      |
      |  projection
ray  /|
    / |
   /__|
  perp
```

**Step 3: Project origin-center perpendicular to axis**
```c
oc_perp = calc_oc_perp(obj, objcent);
```
- Same logic: remove the component along axis
- Now we have 2D problem in the plane perpendicular to axis

**Step 4: Solve 2D circle intersection (quadratic)**
```c
discriminant = calc_disc(ray_perp, oc_perp, obj->data.cy.diameter / 2.0);
```

Inside `calc_disc`:
```c
a = vec_dot(ray_perp, ray_perp);
b = 2.0 * vec_dot(ray_perp, oc_perp);
c = vec_dot(oc_perp, oc_perp) - radius * radius;
return (b * b - 4.0 * a * c);
```
- Same quadratic as sphere, but with perpendicular components
- Tests if ray hits the cylinder's circular cross-section

**Step 5: Check discriminant**
```c
if (discriminant < 0)
    return (-1.0);
```
- No intersection with infinite cylinder

**Step 6: Calculate both intersection points**
```c
calc_intersections(ray_perp, oc_perp, discriminant, obj);
```

Inside `calc_intersections`:
```c
a = vec_dot(ray_perp, ray_perp);
b = 2.0 * vec_dot(ray_perp, oc_perp);
obj->data.cy.t1 = (-b - sqrt(discriminant)) / (2.0 * a);  // Entry point
obj->data.cy.t2 = (-b + sqrt(discriminant)) / (2.0 * a);  // Exit point
```
- Two solutions: where ray enters and exits cylinder

#### Part 2: Height Validation - cy_height()

We found where ray hits infinite cylinder, but is it within our cylinder's height?

```c
void cy_height(t_obj *obj)
{
    obj->t = -1.0;  // Assume no valid hit
```

**Check first intersection (entry)**
```c
h1 = vec_dot(vec_sub(vec_add(obj->ray_origin, vec_mult(obj->ray_dir,
                    obj->data.cy.t1)), obj->data.cy.center),
            obj->data.cy.axis);
```
Let's break this down:
1. `obj->ray_origin + obj->ray_dir * t1` = actual 3D hit point
2. `hit_point - cylinder_center` = vector from center to hit
3. Dot with axis = projection along axis = height coordinate

```c
if (obj->data.cy.t1 > 0 && h1 >= 0 && h1 <= obj->data.cy.height)
    obj->t = obj->data.cy.t1;
```
- If `t1 > 0` (in front of camera)
- And `0 <= h1 <= height` (within cylinder bounds)
- Then this is our hit!

**If first doesn't work, check second (exit)**
```c
else
{
    h2 = vec_dot(vec_sub(vec_add(obj->ray_origin, vec_mult(obj->ray_dir,
                        obj->data.cy.t2)), obj->data.cy.center),
                obj->data.cy.axis);
    if (obj->data.cy.t2 > 0 && h2 >= 0 && h2 <= obj->data.cy.height)
        obj->t = obj->data.cy.t2;
}
```
- Maybe we're inside cylinder looking out
- Or hit from a weird angle

#### Part 3: Cap Testing - cap_calc()

The caps are circles at the top and bottom.

**Setup**
```c
cap_denom = vec_dot(obj->ray_dir, obj->data.cy.axis);
if (fabs(cap_denom) > 1e-6)
```
- Check if ray is parallel to caps (if so, skip)
- `cap_denom` will be used for plane intersection

**Bottom cap**
```c
check_cap(obj, &bottom, obj->data.cy.center, cap_denom);
```

**Top cap**
```c
top_center = vec_add(obj->data.cy.center, 
                    vec_mult(obj->data.cy.axis, obj->data.cy.height));
check_cap(obj, &top, top_center, cap_denom);
```
- Top center = bottom center + axis * height

Inside `check_cap`:

**Step 1: Intersect with cap plane**
```c
cap->t = vec_dot(vec_sub(cap_center, obj->ray_origin), obj->data.cy.axis)
         / cap_denom;
```
- Same as plane intersection
- Cap is a plane perpendicular to axis

**Step 2: Check if intersection is in front**
```c
if (cap->t > 0)
{
```

**Step 3: Find intersection point**
```c
cap->p = vec_add(obj->ray_origin, vec_mult(obj->ray_dir, cap->t));
```

**Step 4: Check if point is within circle radius**
```c
to_center = vec_sub(cap->p, cap_center);
cap->radial = vec_sub(to_center, 
                     vec_mult(obj->data.cy.axis, 
                             vec_dot(to_center, obj->data.cy.axis)));
```
- Remove component along axis (project to cap plane)
- `radial` = distance from axis

```c
if (vec_dot(cap->radial, cap->radial) <= obj->data.cy.diameter
    * obj->data.cy.diameter / 4)
```
- If distance² <= radius², point is inside circle
- This is the cap!

**Step 5: Update closest hit**
```c
if (obj->t < 0 || cap->t < obj->t)
    obj->t = cap->t;
```
- If this cap is closer than body hit, use it

---

## Step 3: Calculate Pixel Color - calc_pixel()

**File:** `src/logic/pixel_calc.c`

```c
void calc_pixel(t_rt *scene)
```

Now we know if we hit something. Time to figure out what color to draw!

### Case 1: We hit an object
```c
if (scene->hit_obj)
{
    hit_calc(scene->hit_obj);   // Calculate hit point and normal
    light_calc(scene);           // Calculate lighting
    draw_pixel(scene);           // Write color to image
}
```

### Case 2: We hit nothing (background)
```c
else
{
    scene->img->pixels[scene->index + 0] = 0;  // Red = 0
    scene->img->pixels[scene->index + 1] = 0;  // Green = 0
    scene->img->pixels[scene->index + 2] = 0;  // Blue = 0
}
```
- Draw black pixel (empty space)

### Always set alpha channel
```c
scene->img->pixels[scene->index + 3] = 255;
```
- Alpha = 255 = fully opaque (not transparent)

---

## Calculate Hit Point and Normal - hit_calc()

**File:** `src/logic/hit_calc.c`

```c
void hit_calc(t_obj *obj)
```

### Calculate where ray hit object
```c
obj->hit_point = vec_add(obj->ray_origin, 
                        vec_mult(obj->ray_dir, obj->closest_t));
```
- Ray equation: `point = origin + direction * t`
- We know `t` (distance), so we calculate exact 3D position

### Calculate surface normal (perpendicular to surface)

#### Sphere normal
```c
if (obj->type == SPHERE)
    obj->normal = vec_normalize(vec_sub(obj->hit_point,
                                       obj->data.sp.center));
```
- Normal at any point on sphere = `(point - center)`
- Normalize to length 1

**Why?** Imagine a ball - at any point on surface, perpendicular points radially outward from center.

#### Plane normal
```c
else if (obj->type == PLANE)
    obj->normal = obj->data.pl.normal;
```
- Plane normal is the same everywhere (already stored)

#### Cylinder normal
```c
else if (obj->type == CYLINDER)
    calc_cylinder_normal(obj);
```

Inside `calc_cylinder_normal`:

**Step 1: Vector from cylinder center to hit point**
```c
hit_to_center = vec_sub(obj->hit_point, obj->data.cy.center);
```

**Step 2: Project onto axis (get height)**
```c
proj = vec_dot(hit_to_center, obj->data.cy.axis);
```
- How far up the cylinder did we hit?

**Step 3: Determine which part we hit**

**Bottom cap (near base)**
```c
if (proj < 0.001)
    obj->normal = vec_negate(obj->data.cy.axis);
```
- Normal points opposite to axis (downward)

**Top cap (near top)**
```c
else if (proj > obj->data.cy.height - 0.001)
    obj->normal = obj->data.cy.axis;
```
- Normal points along axis (upward)

**Body (curved surface)**
```c
else
{
    axis_point = vec_add(obj->data.cy.center,
                        vec_mult(obj->data.cy.axis, proj));
    obj->normal = vec_normalize(vec_sub(obj->hit_point, axis_point));
}
```
- `axis_point` = closest point on cylinder's central axis
- Normal = perpendicular from axis to surface (radial direction)

**Visual:**
```
      |  axis
      |
   ---●---  hit_point
      |
   axis_point

normal points from axis_point → hit_point
```

---

## Calculate Lighting - light_calc()

**File:** `src/logic/light_calc.c`

```c
void light_calc(t_rt *scene)
{
    setup_light_dir(scene->hit_obj, scene->light->pos);
    scene->hit_obj->in_shadow = check_shadow(scene);
    calc_brightness(scene);
}
```

Three steps: find light direction, check shadows, calculate brightness.

### Step 1: Setup Light Direction - setup_light_dir()

```c
static void setup_light_dir(t_obj *hit_obj, t_vec3 light_pos)
{
    hit_obj->light_dir = vec_sub(light_pos, hit_obj->hit_point);
    hit_obj->light_dist = vec_length(hit_obj->light_dir);
    hit_obj->light_dir = vec_normalize(hit_obj->light_dir);
}
```

**Line 1: Vector from hit point to light**
```c
hit_obj->light_dir = vec_sub(light_pos, hit_obj->hit_point);
```
- Direction we need to check for shadows

**Line 2: Distance to light**
```c
hit_obj->light_dist = vec_length(hit_obj->light_dir);
```
- How far away is the light?
- We'll need this to check if something blocks it

**Line 3: Normalize direction**
```c
hit_obj->light_dir = vec_normalize(hit_obj->light_dir);
```
- Make it length 1 for lighting calculations

### Step 2: Check for Shadows - check_shadow()

```c
static bool check_shadow(t_rt *scene)
{
    shadow_origin = vec_add(scene->hit_obj->hit_point,
                           vec_mult(scene->hit_obj->normal, 0.001));
```

**Why `+ normal * 0.001`?**
- We start shadow ray slightly ABOVE surface (epsilon offset)
- Prevents "self-intersection" bug (ray hitting the object it started from)
- `0.001` is tiny (1mm in world units)

**Loop through all objects**
```c
shadow_obj = scene->objects;
while (shadow_obj)
{
    shadow_obj->ray_origin = shadow_origin;
    shadow_obj->ray_dir = scene->hit_obj->light_dir;
    shadow_t = get_shadow_t(shadow_obj);
```
- Shoot ray from hit point toward light
- Test if it hits any object

**Check if something blocks the light**
```c
if (shadow_t > 0.001 && shadow_t < scene->hit_obj->light_dist)
    return (true);
```
- If we hit something between surface and light → shadow!
- `> 0.001` prevents self-intersection again
- `< light_dist` ensures object is between us and light (not behind light)

**No blockers found**
```c
return (false);
```
- Light reaches surface directly

### Step 3: Calculate Brightness - calc_brightness()

```c
static void calc_brightness(t_rt *scene)
{
```

**Calculate diffuse lighting (Lambertian)**
```c
scene->hit_obj->diffuse = fmax(0.0, vec_dot(scene->hit_obj->normal,
                                           scene->hit_obj->light_dir));
```
- **Dot product** of normal and light direction
- Measures "how directly" light hits surface
- Positive = light hitting front, 0 = perpendicular, negative = hitting back
- `fmax(0.0, ...)` clamps negative to 0 (no negative light)

**Lighting scenarios:**
```
Light directly above:
   ↓ light
   |
---●--- surface
   ↓ normal
dot(normal, light) = 1.0 (maximum brightness)

Light at 45° angle:
  ↓ light
 /
●--- surface
↓ normal
dot(normal, light) = 0.707 (less bright)

Light perpendicular:
→ light   ↓ normal
          |
        --●--
dot(normal, light) = 0 (no light, grazing angle)
```

**Apply shadow**
```c
if (scene->hit_obj->in_shadow)
    scene->hit_obj->diffuse = 0.0;
```
- In shadow? No direct light, so diffuse = 0

**Combine ambient and diffuse**
```c
scene->hit_obj->brightness = scene->ambient->ratio
    + (scene->light->brightness * scene->hit_obj->diffuse);
```
- **Ambient** = base light everywhere (e.g., 0.2 = 20% brightness minimum)
- **Diffuse** = directional light based on angle and shadow
- Total brightness = ambient + (light_intensity × diffuse)

**Clamp to valid range**
```c
scene->hit_obj->brightness = fmin(scene->hit_obj->brightness, 1.0);
```
- Maximum brightness is 1.0 (100%)
- Prevents overexposure

---

## Draw the Pixel - draw_pixel()

**File:** `src/logic/pixel_calc.c`

```c
void draw_pixel(t_rt *scene)
{
```

### Get object's color
```c
if (scene->hit_obj->type == SPHERE)
    color = &scene->hit_obj->data.sp.color;
else if (scene->hit_obj->type == PLANE)
    color = &scene->hit_obj->data.pl.color;
else
    color = &scene->hit_obj->data.cy.color;
```
- Each object type stores color in different place (union)
- `color` is now a pointer to RGB values (0-255)

### Write to image buffer
```c
scene->img->pixels[scene->index + 0] = (int)(color->r * scene->hit_obj->brightness);
scene->img->pixels[scene->index + 1] = (int)(color->g * scene->hit_obj->brightness);
scene->img->pixels[scene->index + 2] = (int)(color->b * scene->hit_obj->brightness);
```
- Multiply base color by brightness
- Example: Red sphere (255, 0, 0) with brightness 0.5 → (127, 0, 0) dark red
- `(int)` casts float to integer (pixels are bytes 0-255)
- `index + 0/1/2` = Red/Green/Blue channels

**Why multiply?**
- Brightness modulates color intensity
- Dark areas: brightness near 0 → dark color
- Bright areas: brightness near 1 → full color
- Shadows: brightness = ambient only → very dark

---

## Back to Main Loop

After `render_pixel` finishes for all 480,000 pixels, we're done rendering!

### Display the image
```c
mlx_image_to_window(scene->window, scene->img, 0, 0);
```
- Transfer our pixel buffer to the window
- Position (0, 0) = top-left corner
- Now we can see our rendered scene!

### Setup event hooks
```c
mlx_key_hook(scene->window, key_hook, scene->window);
```
- Register keyboard handler
- `key_hook` function will be called when keys pressed

**File:** `src/hooks.c`
```c
void key_hook(mlx_key_data_t keydata, void *param)
{
    if (keydata.key == MLX_KEY_ESCAPE && keydata.action == MLX_PRESS)
        mlx_close_window(mlx);
}
```
- If ESC pressed, close window and exit

```c
mlx_resize_hook(scene->window, resize_hook, scene);
```
- Register resize handler
- Called whenever window dimensions change

**File:** `src/hooks.c`
```c
void resize_hook(int32_t width, int32_t height, void *param)
{
    scene->width = width;
    scene->height = height;
    mlx_resize_image(scene->img, width, height);
    pre_calc_camera(scene);  // Recalculate viewport for new aspect ratio
    render_scene(scene);      // Re-render entire scene
}
```
- Update dimensions
- Resize image buffer
- Re-render everything for new size

### Enter event loop
```c
mlx_loop(scene->window);
```
- **Blocks here** until window closes
- MLX42 handles:
  - Displaying the window
  - Processing events (keyboard, mouse, resize)
  - Calling our hook functions when needed

### Cleanup
```c
mlx_delete_image(scene->window, scene->img);
mlx_terminate(scene->window);
```
- Free image buffer
- Close window and cleanup MLX resources

**Then back to main for final cleanup:**
```c
free_scenes(scene);
return (0);
```

---

## Summary: Complete Flow

Here's the full journey for one pixel:

```
1. main() → window_loop() → render_scene()
   ↓
2. render_pixel(i, j)
   ↓
3. calc_camera(i, j)
   - Convert pixel (j, i) to viewport position
   - Calculate ray direction through that point
   - Store ray in scene->objects->ray_dir
   ↓
4. calc_objs()
   - Loop through all objects
   - For each object:
     * Call hit_sphere() / hit_plane() / hit_cylinder()
     * Calculate intersection distance t
     * Track closest hit
   - Store closest object in scene->hit_obj
   ↓
5. calc_pixel()
   ↓
   If hit something:
     ↓
     5a. hit_calc()
         - Calculate exact hit point (origin + dir * t)
         - Calculate surface normal (perpendicular)
     ↓
     5b. light_calc()
         - Calculate direction to light
         - Check if anything blocks light (shadow)
         - Calculate brightness (ambient + diffuse)
     ↓
     5c. draw_pixel()
         - Get object's color
         - Multiply by brightness
         - Write RGB to image buffer
   
   If hit nothing:
     - Write black (0, 0, 0) to image buffer
   ↓
6. Repeat for next pixel...
```

After all pixels rendered:
```
7. Display image on screen
8. Wait for events (ESC to exit, resize, etc.)
9. Cleanup and exit
```

---

## Key Concepts Recap

### Vectors
- **3D points** in space: (x, y, z)
- Can be added, subtracted, scaled
- **Length** = distance from origin
- **Normalize** = make length 1 (direction only)
- **Dot product** = measures alignment (-1 to 1)
- **Cross product** = creates perpendicular vector

### Rays
- **Origin** + **Direction** × **t** = point on ray
- Like a laser beam - starts somewhere, goes in a direction
- `t` = distance traveled along ray

### Ray Tracing
1. For each pixel, shoot ray from camera
2. Find closest object ray hits
3. Calculate how light affects that spot
4. Color pixel accordingly

### Lighting Model
- **Ambient** = base light everywhere (like moonlight)
- **Diffuse** = directional light based on angle (like sunlight)
- **Shadow** = something blocks light
- **Brightness** = ambient + diffuse (if not in shadow)

### Why It's Slow
- For 800×600 image = 480,000 pixels
- Each pixel tests EVERY object
- Each test involves expensive math (sqrt, dot products)
- Shadow ray doubles the work (test all objects again)
- No GPU acceleration

**Performance note:** On my machine, a simple scene (10 objects) takes ~0.1 seconds. Complex scenes (100+ objects) can take several seconds. Resizing re-renders everything, which is why it lags.

---

## Next Steps to Understanding

If you want to learn more:

1. **Play with the math** - Change FOV, move camera, see how rays change
2. **Add debug output** - Print ray directions, hit points, distances
3. **Visualize** - Draw rays in 2D to see intersection geometry
4. **Optimize** - Learn about spatial data structures (BVH, octrees)
5. **Add features** - Reflections, refractions, more lights, textures

The best way to understand is to break things and fix them. Try changing values, comment out code, see what breaks. That's how you learn what each part does!
