# OpenGL3DEngine Grassy Field
<img src="grassgif.gif" alt="grassgif" height="500"/>
Experimental 3d engine made with OpenGL that I used to render a very very large field of grass blowing in the wind.

Optimizations:
1. Used instanced rendering which allows me to make just one rendering call for entire chunks of grass. This could be optimized even further as I split the world up into chunks for the purpose of improving load time. Each chunk now makes its own instanced rendering call to render all of its grass which is a lot less efficicent than having a single instanced rendering call for all of the grass at once.
2. I added CPU side frustum culling where chunks outside of the camera's view do not make rendering calls at all. The graphics card already does culling by itself but this occurs in the rasterization state which is after the vertex shader so its a lot faster to cull out non-visible grass before it even makes it to the GPU.
3. Added level of detail changes where at a certain distance, grass is no longer rendered but the terrain underneath is still rendered as it still gives the illusion of more grassy hills far in the distance.

The actual generation of the grassy field is entirely procedural using perlin noise maps to move the grass and terrain up or down. The wind was implemented using a simple sum of sines with some added randomness. The normal vector is recalculated in the vertex shader by just simulating the wind transformation a point slightly above the vertex. 
As for the fragment shader, it was only used for the Phong model, fog, and a skybox using cube maps.

VIDEO DEMO:
https://youtu.be/ajOxgA49QB8?si=SipAh-Mga4zKOfHz
