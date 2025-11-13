/*
i have a 3d space.
i have objects in that 3d space
the camera, the light source and the shapes are all objects
from the light source to the object is a ray, a vector
from the camera, within its pov, to the object is a ray.
i want to use the light rays and the math library to:
figure out where a lightray hits an object, where it bounces to.
i only need to calculate the crossing rays between camera and light ray.
through the calc we get the color, brightness and vector directions
what do i need for that?
for a single pixel:
the camera sends out a ray for that pixel, that ray hits an object.
we see if from that intersection, that crosspoint,
a lightray hits directly from the lightsource, if not its darker or black.
for simplicity, without a loop or anything, just a plane:
we send light from our lightsource -> the ray hits the plane, 
through vector math we know if it will cross the camera ray,
if it does we can see the pixel in its color.

in a loop we do the same for each pixel:
light ray goes out, we check where it bounces to and if it hits another surface,
for every pixel it intersects with the camera we can display that pixel to it.


A ray is mathematically represented as:
r(t)=o+t⋅d
Where:
o = ray origin (camera position, light source, etc.)
d = ray direction (normalized vector)
t ≥ 0 = distance parameter along the ray
You can think of t as how far along the ray you move.


he first step is finding where a ray hits an object (if at all).
For example, with a sphere (center C, radius r):
∥o+td−C∥^2=r^2
Expanding gives a quadratic in t.
If you get one or two positive t values, those are intersection points.
For a plane (point P₀, normal n):
t=(P₀−o)⋅n / d⋅n
If t > 0, you hit the plane at:
P=o+td

Here’s the “mental flip” moment:
You don’t need to shoot rays from the light source.
Instead, you shoot one ray per pixel from the camera, and for each ray:
Find the nearest intersection with any object.
Compute the point of intersection P.
From P, shoot a shadow ray toward the light.
If the shadow ray hits anything before reaching the light → the point is in shadow.
If not → it’s lit, and you compute the brightness based on light angle and surface normal.

Lighting at a Point (Phong or Lambertian)

At point 
P on a surface with normal n, light direction L, and view direction V:

Diffuse lighting (Lambert):
I(diffuse)=k⋅max(0,n⋅L)

Specular lighting (Phong):
I(specular)=k⋅max(0,R⋅V)^α

where R is the reflection of L about n.

Total color:
I=I(ambient)+I(diffuse)+I(specular)


Summary: What You Need to Implement
Ray structure → origin + direction
Vector math functions → dot, cross, normalize, reflect
Intersection tests → sphere, plane, etc.
Camera setup → compute direction for each pixel ray
Lighting → compute normal, shadow ray, diffuse/specular components
Render loop → one ray per pixel, find color, write to image buffer


Conceptual Shortcut
A camera ray asks: “What color do I see in this direction?”
To answer that, you:
Find the closest object in that direction
Check if it’s lit or shadowed
Compute its color based on light and material
You only cast rays from the camera, but at each hit point, you query light visibility.

*/