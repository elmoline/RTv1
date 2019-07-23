/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wael-mos <wael-mos@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/23 16:05:37 by wael-mos          #+#    #+#             */
/*   Updated: 2019/07/22 16:34:36 by evogel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

int        cylinder(t_ray *r, t_sph *s, float *t)
{
	t_vec rot;
	t_vec dist;
	int ret = 0;
	float    A;
	float    B;
	float    C;
	float    discr;

	s->radius = 2;
	rot = vec(1, 0, 0.5);

	/* https://mrl.nyu.edu/~dzorin/rend05/lecture2.pdf page 2
	 *      * No idea if i did it well still doing test */

	A = dot(&r->dir, &r->dir) - pow(dot(&r->dir, &rot), 2);

	dist = sub_vec(&r->start, &s->pos);
	printf("%f\n", dist.z);
	B = 2 * (dot(&r->dir, &dist) - dot(&r->dir, &rot)) * dot(&dist, &rot);

	C = dot(&dist, &dist) - pow(dot(&dist, &rot), 2) - 1; /* or pow(s->radius, 2) */
	discr = B * B - 4 * A * C;
	if (discr < 0)
		ret = 0;
	else
	{
		float sqrtdiscr;
		float t0;
		float t1;

		sqrtdiscr = sqrtf(discr);
		t0 = (-B + sqrtdiscr)/(2 * A);
		t1 = (-B - sqrtdiscr)/(2 * A);

		/* We want the closest one */
		if (t0 > t1)
			t0 = t1;

		/* Verify t1 larger than 0 and less than the original t */
		if ((t0 > 0.001f) && (t0 < *t))
		{
			*t = t0;
			ret = 1;
		}
		else
			ret = 0;
	}
	return (ret);
}

/* Check if the ray and sphere intersect */
int sphere(t_ray *r, t_sph *s, float *t)
{
	/* A = d.d, the vector dot product of the direction */
	float A; 
	A = dot(&r->dir, &r->dir); 

	/* We need a vector representing the distance between the start of 
	 * the ray and the position of the circle.
	 * This is the term (p0 - c) 
	 */
	t_vec dist;
	dist = sub_vec(&r->start, &s->pos);

	/* 2d.(p0 - c) */  
	float B;
	B = 2 * dot(&r->dir, &dist);

	/* (p0 - c).(p0 - c) - r^2 */
	float C;
	C = dot(&dist, &dist) - (s->radius * s->radius);

	/* Solving the discriminant */
	float discr;
	discr = B * B - 4 * A * C;

	/* If the discriminant is negative, there are no real roots.
	 * Return false in that case as the ray misses the sphere.
	 * Return true in all other cases (can be one or two intersections)
	 * t represents the distance between the start of the ray and
	 * the point on the sphere where it intersects.
	 */
	if (discr < 0)
		return (0);
	else
	{
		float t0;
		float t1;

		t0 = (-B + sqrtf(discr))/2;
		t1 = (-B - sqrtf(discr))/2;

		/* We want the closest one */
		if (t0 > t1)
			t0 = t1;

		/* Verify t1 larger than 0 and less than the original t */
		if ((t0 > 0.001f) && (t0 < *t))
		{
			*t = t0;
			return (1);
		}
	}
	return (0);
}

int		toomuch_lineagain(t_mlx *data)
{
	t_ray r;
	unsigned int number_mat;

	number_mat = 4;
	t_sph spheres[number_mat];

	spheres[0].pos.x = 450;
	spheres[0].pos.y = 350;
	spheres[0].pos.z = 300;
	spheres[0].radius = 100;
	spheres[0].material.diffuse.red = 1;
	spheres[0].material.diffuse.green = 0;
	spheres[0].material.diffuse.blue = 1;
	spheres[0].material.reflection = 1;

	spheres[1].pos.x = 350;
	spheres[1].pos.y = 500;
	spheres[1].pos.z = 0;
	spheres[1].radius = 50;
	spheres[1].material.diffuse.red = 0;
	spheres[1].material.diffuse.green = 1;
	spheres[1].material.diffuse.blue = 1;
	spheres[1].material.reflection = 0.3;

	spheres[2].pos.x = 350;
	spheres[2].pos.y = 350;
	spheres[2].pos.z = 100;
	spheres[2].radius = 70;
	spheres[2].material.diffuse.red = 1;
	spheres[2].material.diffuse.green = 1;
	spheres[2].material.diffuse.blue = 1;
	spheres[2].material.reflection = 0.2;

	spheres[3].pos.x = 200;
	spheres[3].pos.y = 150;
	spheres[3].pos.z = 500;
	spheres[3].radius = 100;
	spheres[3].material.diffuse.red = 1;
	spheres[3].material.diffuse.green = 1;
	spheres[3].material.diffuse.blue = 0;
	spheres[3].material.reflection = 0.6;

	t_light lights[2];
	lights[0].pos.x = 700;
	lights[0].pos.y = 700;
	lights[0].pos.z = -500;
	lights[0].intensity.red = 1;
	lights[0].intensity.green = 1;
	lights[0].intensity.blue = 0.8;

	lights[1].pos.x = 0;
	lights[1].pos.y = 0;
	lights[1].pos.z = -300;
	lights[1].intensity.red = 0.7;
	lights[1].intensity.green = 0;
	lights[1].intensity.blue = 0.6;

	/* Will contain the raw image */
	int x;
	int y;
	y = 0;
	while (y < WIN_Y)
	{
		x = 0;
		while (x < WIN_X)
		{

			float red;
			float green;
			float blue;

			red = 0;
			green = 0;
			blue = 0;

			int level;
			float coef;

			level = 0;
			coef = 1.0;

			r.start.x = x;
			r.start.y = y;
			r.start.z = -2000;

			r.dir.x = 0;
			r.dir.y = 0;
			r.dir.z = 1;

			while ((coef > 0.0f) && (level < 5))
			{
				/* Find closest intersection */
				float t;
				int currentSphere;

				t = 20000.0f;
				currentSphere = -1;

				unsigned int i;
				i = 0;

				while (i < number_mat)
				{
					if (sphere(&r, &spheres[i], &t))
						currentSphere = i;
					i++;
				}
				if (currentSphere == (int)number_mat)
					break;

				t_vec scaled;
				t_vec newStart;

				scaled = scale(t, &r.dir);
				newStart = add_vec(&r.start, &scaled);

				/* Find the normal for this new vector at the point of intersection */
				t_vec n;
				float temp;

				n = sub_vec(&newStart, &spheres[currentSphere].pos);
				temp = dot(&n, &n);
				if (temp == 0)
					break;

				temp = 1.0f / sqrtf(temp);
				n = scale(temp, &n);

				/* Find the material to determine the colour */
				t_mat currentMat;
				currentMat = spheres[currentSphere].material;

				/* Find the value of the light at this point */
				unsigned int j;
				j = 0;

				while (j++ < 2)
				{
					t_vec dist;
					t_light light;

					light = lights[j - 1];
					dist = sub_vec(&light.pos, &newStart);
					if (dot(&n, &dist) <= 0.0f)
						continue;
					float t;
					t = sqrtf(dot(&dist,&dist));
					if (t <= 0.0f)
						continue;

					t_ray lightRay;
					lightRay.start = newStart;
					lightRay.dir = scale((1/t), &dist);

					/* Calculate shadows */
					int inShadow;
					unsigned int k;

					inShadow = 0;
					k = 0;
					while (++k < number_mat)
					{
						if (sphere(&lightRay, &spheres[k], &t))
						{
							inShadow = 1;
							break;
						}
					}
					if (!inShadow)
					{
						/* Lambert diffusion */
						float lambert;
						lambert = dot(&lightRay.dir, &n) * coef; 
					//	if (lambert < 0.0f)
					//		lambert = 0.0f;
						red += (0.95f * lambert * light.intensity.red + 0.05f) * currentMat.diffuse.red;
						green += (0.95f * lambert * light.intensity.green + 0.05f) * currentMat.diffuse.green;
						blue += (0.95f * lambert * light.intensity.blue + 0.05f) * currentMat.diffuse.blue;
					}
				}
				/* Iterate over the reflection */
				coef *= currentMat.reflection;

				/* The reflected ray start and direction */
				float reflect;
				t_vec tmp;

				r.start = newStart;
				reflect = 2.0f * dot(&r.dir, &n);
				tmp = scale(reflect, &n);
				r.dir = sub_vec(&r.dir, &tmp);

				level++;

			}
			int color;
			color = pow(256,2) * (unsigned char)min(red * 255.0f, 255.0f) + 256 * (unsigned char)min(green * 255.0f, 255.0f) + (unsigned char)min(blue * 255.0f, 255.0f);
			data->data[(x + y * WIN_X) + 0] = color;
			x++;
		}
		y++;
	}
	mlx_put_image_to_window(data->mlx_ptr, data->win_ptr, data->image_ptr, 0, 0);
	return (0);
}

int	main(void)
{
	t_mlx	data;

	data.mlx_ptr = mlx_init();
	data.win_ptr = mlx_new_window(data.mlx_ptr, WIN_X, WIN_Y, "RTv1");
	data.image_ptr = mlx_new_image(data.mlx_ptr, WIN_X, WIN_Y);
	data.data = (int *)mlx_get_data_addr(data.image_ptr, &data.bpp,\
			&data.size_line, &data.end);
	mlx_loop_hook(data.mlx_ptr, toomuch_lineagain, &data);
	mlx_hook(data.win_ptr, 17, (1L << 17), deal_close, NULL);
	mlx_hook(data.win_ptr, 2, 0, deal_key, &data);
	mlx_loop(data.mlx_ptr);
	return (0);
}
