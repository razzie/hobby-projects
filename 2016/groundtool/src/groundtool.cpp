#include <algorithm>
#include <ctime>
#include <cstdint>
#include <iostream>
#include <functional>
#include <map>
#include <vector>
#include "irrlicht.h"

using namespace irr;

class EventReceiver : public IEventReceiver
{
public:
	typedef unsigned CallbackID;
	typedef std::function<bool(const SEvent::SKeyInput&)> KeyCallback;
	typedef std::function<bool(const SEvent::SMouseInput&)> MouseCallback;

	EventReceiver()
	{
	}

	virtual ~EventReceiver()
	{
	}

	CallbackID addKeyCallback(KeyCallback cb)
	{
		if (cb)
		{
			m_key_cbs.emplace(++m_curr_id, cb);
			return m_curr_id;
		}
		else
			return 0;
	}

	CallbackID addMouseCallback(MouseCallback cb)
	{
		if (cb)
		{
			m_mouse_cbs.emplace(++m_curr_id, cb);
			return m_curr_id;
		}
		else
			return 0;
	}

	void removeCallback(CallbackID id)
	{
		m_key_cbs.erase(id);
		m_mouse_cbs.erase(id);
	}

	void removeCallbacks()
	{
		m_key_cbs.clear();
		m_mouse_cbs.clear();
	}

	// inherited from IEventReceiver
	virtual bool OnEvent(const SEvent& event) // return true to PREVENT further processing of the event
	{
		switch (event.EventType)
		{
		case EET_KEY_INPUT_EVENT:
			for (auto& it : m_key_cbs)
			{
				if (it.second(event.KeyInput)) return true;
			}
			return false;

		case EET_MOUSE_INPUT_EVENT:
			for (auto& it : m_mouse_cbs)
			{
				if (it.second(event.MouseInput)) return true;
			}
			return false;

		default:
			return false;
		}
	}

private:
	CallbackID m_curr_id;
	std::map<CallbackID, KeyCallback> m_key_cbs;
	std::map<CallbackID, MouseCallback> m_mouse_cbs;
};

class MouseController
{
private:
	core::vector2di m_last_pos;
	scene::ISceneManager* m_smgr;
	EventReceiver* m_evtrec;
	EventReceiver::CallbackID m_cb_id;

	static void rotateAroundCentre(core::vector3df& point, core::vector3df center, core::vector3df rotation)
	{
		point -= center;
		core::matrix4 m;
		m.setRotationDegrees(rotation);
		m.rotateVect(point);
		point += center;
	}

	static void rotateVectorAroundAxis(core::vector3df& vector, const core::vector3df& axis, f32 radians)
	{
		core::quaternion q;
		core::matrix4 m;
		q.fromAngleAxis(radians, axis);
		q.getMatrix(m, core::vector3df(0, 0, 0));
		m.rotateVect(vector);
	}

public:
	bool onMouseEvent(const SEvent::SMouseInput& event)
	{
		core::vector2di curr_pos(event.X, event.Y);
		core::vector2di delta = curr_pos - m_last_pos;
		scene::ICameraSceneNode* cam = m_smgr->getActiveCamera();
		float speed = 1.f;

		if (event.isRightPressed())
		{
			core::vector3df pos = cam->getPosition();
			core::vector3df target = cam->getTarget();
			core::vector3df right = core::vector3df(0.f, 1.f, 0.f).crossProduct(target - pos);
			right.normalize();

			pos -= target;
			rotateVectorAroundAxis(pos, right, speed * delta.Y * core::DEGTORAD);
			pos += target;
			pos.rotateXZBy(speed * -delta.X, target);

			cam->setPosition(pos);
		}

		if (event.Wheel > 0)
		{
			core::vector3df cam_pos = cam->getPosition();
			cam_pos *= 0.9f;
			cam->setPosition(cam_pos);
		}
		else if (event.Wheel < 0)
		{
			core::vector3df cam_pos = cam->getPosition();
			cam_pos *= 1.1f;
			cam->setPosition(cam_pos);
		}

		m_last_pos = curr_pos;

		return false;
	}

	MouseController(scene::ISceneManager* smgr, EventReceiver* evtrec) : 
		m_last_pos(0, 0), m_smgr(smgr), m_evtrec(evtrec)
	{
		m_cb_id = m_evtrec->addMouseCallback(std::bind(&MouseController::onMouseEvent, this, std::placeholders::_1));
	}

	~MouseController()
	{
		m_evtrec->removeCallback(m_cb_id);
	}
};

class Triangulate
{
public:
	// compute area of a contour/polygon
	static float getArea(const std::vector<core::vector2df>& contour)
	{
		int n = contour.size();

		float A = 0.0f;

		for (int p = n - 1, q = 0; q<n; p = q++)
		{
			A += contour[p].X * contour[q].Y - contour[q].X * contour[p].Y;
		}
		return A * 0.5f;
	}

	// decide if point Px/Py is inside triangle defined by
	// (Ax,Ay) (Bx,By) (Cx,Cy)
	static bool isInsideTriangle(float Ax, float Ay,
		float Bx, float By,
		float Cx, float Cy,
		float Px, float Py)
	{
		float ax, ay, bx, by, cx, cy, apx, apy, bpx, bpy, cpx, cpy;
		float cCROSSap, bCROSScp, aCROSSbp;

		ax = Cx - Bx;  ay = Cy - By;
		bx = Ax - Cx;  by = Ay - Cy;
		cx = Bx - Ax;  cy = By - Ay;
		apx = Px - Ax;  apy = Py - Ay;
		bpx = Px - Bx;  bpy = Py - By;
		cpx = Px - Cx;  cpy = Py - Cy;

		aCROSSbp = ax*bpy - ay*bpx;
		cCROSSap = cx*apy - cy*apx;
		bCROSScp = bx*cpy - by*cpx;

		return ((aCROSSbp >= 0.0f) && (bCROSScp >= 0.0f) && (cCROSSap >= 0.0f));
	}

	// triangulate a contour/polygon, places results in index vector
	template<class T = std::vector<u16>>
	static bool process(const std::vector<core::vector2df>& contour, T& indices)
	{
		/* allocate and initialize list of Vertices in polygon */

		int n = contour.size();
		if (n < 3) return false;

		int *V = new int[n];

		/* we want a counter-clockwise polygon in V */

		if (0.0f < getArea(contour))
			for (int v = 0; v<n; v++) V[v] = v;
		else
			for (int v = 0; v<n; v++) V[v] = (n - 1) - v;

		int nv = n;

		/*  remove nv-2 Vertices, creating 1 triangle every time */
		int count = 2 * nv;   /* error detection */

		for (int m = 0, v = nv - 1; nv>2;)
		{
			/* if we loop, it is probably a non-simple polygon */
			if (0 >= (count--))
			{
				//** Triangulate: ERROR - probable bad polygon!
				return false;
			}

			/* three consecutive vertices in current polygon, <u,v,w> */
			int u = v; if (nv <= u) u = 0;     /* previous */
			v = u + 1; if (nv <= v) v = 0;     /* new v    */
			int w = v + 1; if (nv <= w) w = 0;     /* next     */

			if (snips(contour, u, v, w, nv, V))
			{
				int a, b, c, s, t;

				/* true names of the vertices */
				a = V[u]; b = V[v]; c = V[w];

				/* output Triangle */
				indices.push_back(c);
				indices.push_back(b);
				indices.push_back(a);

				m++;

				/* remove v from remaining polygon */
				for (s = v, t = v + 1; t<nv; s++, t++) V[s] = V[t]; nv--;

				/* resest error detection counter */
				count = 2 * nv;
			}
		}

		delete V;

		return true;
	}

private:
	static bool snips(const std::vector<core::vector2df>& contour, int u, int v, int w, int n, int *V)
	{
		const float EPSILON = 0.0000000001f;
		int p;
		float Ax, Ay, Bx, By, Cx, Cy, Px, Py;

		Ax = contour[V[u]].X;
		Ay = contour[V[u]].Y;

		Bx = contour[V[v]].X;
		By = contour[V[v]].Y;

		Cx = contour[V[w]].X;
		Cy = contour[V[w]].Y;

		if (EPSILON > (((Bx - Ax)*(Cy - Ay)) - ((By - Ay)*(Cx - Ax)))) return false;

		for (p = 0; p<n; p++)
		{
			if ((p == u) || (p == v) || (p == w)) continue;
			Px = contour[V[p]].X;
			Py = contour[V[p]].Y;
			if (isInsideTriangle(Ax, Ay, Bx, By, Cx, Cy, Px, Py)) return false;
		}

		return true;
	}
};

core::vector3df getRandVect(float scale = 1.f)
{
	float rX = (float)((rand() % 201) - 100) / 100.f;
	float rY = (float)((rand() % 201) - 100) / 100.f;
	float rZ = (float)((rand() % 201) - 100) / 100.f;
	return{ rX * scale, rY * scale, rZ * scale };
}

void subdivide(scene::IVertexBuffer& vertices, scene::IIndexBuffer& indices, f32 max_area = 25.f)
{
	for (size_t i = 0; i < indices.size(); i += 3)
	{
		u16 i1 = indices[i];
		u16 i2 = indices[i + 1];
		u16 i3 = indices[i + 2];
		core::vector2df p1(vertices[i1].Pos.X, vertices[i1].Pos.Z);
		core::vector2df p2(vertices[i2].Pos.X, vertices[i2].Pos.Z);
		core::vector2df p3(vertices[i3].Pos.X, vertices[i3].Pos.Z);
		f32 a = p2.getDistanceFrom(p3);
		f32 b = core::line2df(p2, p3).getClosestPoint(p1).getDistanceFrom(p1);
		f32 area = (a * b) / 2.f;

		if (area > max_area && a > 5.f && b > 5.f)
		{
			// calculating steiner point data
			video::SColor color = vertices[i1].Color;
			core::vector2df center = (p1 + p2 + p3) / 3.f;
			core::vector3df p3d(center.X, 0.f, center.Y);
			if (a > 10.f && b > 10.f) p3d += getRandVect(1.f);

			// add steiner point
			vertices.push_back({ p3d, { 0.f, 1.f, 0.f }, color, { 0, 0 } });
			u16 center_index = vertices.size() - 1;

			// triangle 1
			indices.setValue(i, center_index);
			indices.setValue(i + 1, i2);
			indices.setValue(i + 2, i3);

			// triangle 2
			indices.push_back(center_index);
			indices.push_back(i3);
			indices.push_back(i1);

			// triangle 3
			indices.push_back(center_index);
			indices.push_back(i1);
			indices.push_back(i2);

			// recheck for triangle 1
			i -= 3;
		}
	}
}

inline core::vector3df to3d(core::vector2df v)
{
	return{ v.X, 0.f, v.Y };
}

class GroundShaderCallback : public video::IShaderConstantSetCallBack
{
public:
	GroundShaderCallback(scene::ISceneManager* smgr) : m_smgr(smgr)
	{
	}

	virtual void OnSetConstants(video::IMaterialRendererServices* services, s32 userData)
	{
		video::IVideoDriver* driver = services->getVideoDriver();

		core::matrix4 world = driver->getTransform(video::ETS_WORLD);
		services->setVertexShaderConstant("world", world.pointer(), 16);

		core::matrix4 world_view_proj;
		world_view_proj = driver->getTransform(video::ETS_PROJECTION);
		world_view_proj *= driver->getTransform(video::ETS_VIEW);
		world_view_proj *= driver->getTransform(video::ETS_WORLD);
		services->setVertexShaderConstant("world_view_proj", world_view_proj.pointer(), 16);

		//core::vector3df light_pos(50.f, 50.f, 0.f);
		core::vector3df light_pos = m_smgr->getActiveCamera()->getPosition();
		services->setVertexShaderConstant("light_pos", reinterpret_cast<f32*>(&light_pos), 3);
		
		video::SColorf light_color(1.f, 1.f, 1.f);
		services->setVertexShaderConstant("light_color", reinterpret_cast<f32*>(&light_color), 4);
	}

private:
	scene::ISceneManager* m_smgr;
};

class Ground : public scene::ISceneNode
{
private:
	video::SMaterial m_material;
	scene::CVertexBuffer m_vertices;
	scene::CIndexBuffer m_indices;
	scene::CDynamicMeshBuffer m_meshbuffer;

public:
	Ground(scene::ISceneManager* smgr) :
		scene::ISceneNode(smgr->getRootSceneNode(), smgr),
		m_vertices(video::EVT_STANDARD),
		m_indices(video::EIT_16BIT),
		m_meshbuffer(video::EVT_STANDARD, video::EIT_16BIT)
	{
		video::IGPUProgrammingServices* gpu = getSceneManager()->getVideoDriver()->getGPUProgrammingServices();
		GroundShaderCallback* shader_cb = new GroundShaderCallback(getSceneManager());
		s32 mat_type = gpu->addHighLevelShaderMaterialFromFiles(
			"../shaders/ground.vert", "main", video::EVST_VS_1_1,
			"../shaders/ground.frag", "main", video::EPST_PS_1_1,
			shader_cb);
		m_material.MaterialType = (video::E_MATERIAL_TYPE)mat_type;

		//m_material.Lighting = false;
		//m_material.GouraudShading = false;
		//m_material.BackfaceCulling = false;
		//m_material.NormalizeNormals = true;

		m_meshbuffer.setVertexBuffer(&m_vertices);
		m_meshbuffer.setIndexBuffer(&m_indices);
	}

	bool init(std::vector<core::vector2df>& contour)
	{
		m_vertices.set_used(0);
		m_indices.set_used(0);

		// reverse order if not clockwise
		if (0.0f > Triangulate::getArea(contour))
			std::reverse(contour.begin(), contour.end());

		if (!Triangulate::process(contour, m_indices))
			return false;

		// ground surface
		for (auto& p : contour)
			m_vertices.push_back({ { p.X, 0.f, p.Y }, { 0.f, 1.f, 0.f }, { 255, 0, 128, 0 }, { 0.f, 0.f } });

		subdivide(m_vertices, m_indices);

		// calculating center
		core::vector2df center;
		for (auto& p : contour) center += p;
		center /= contour.size();
		//core::vector3df endpos(center.X, -30.f, center.Y);

		// thick edge of surface
		for (size_t i = 0, n = contour.size(), base_index = m_vertices.size(); i < n; ++i)
		{
			video::S3DVertex v;
			v.Pos = m_vertices[i].Pos;// + getRandVect(0.5f);
			v.Pos.X = (v.Pos.X - center.X) * 1.05f + center.X;
			v.Pos.Z = (v.Pos.Z - center.Y) * 1.05f + center.Y;
			v.Pos.Y -= 2.0f;
			v.Pos += getRandVect(0.5f);
			v.Normal = core::vector3df(v.Pos.X, 0.f, v.Pos.Y).normalize();
			v.Color = { 255, 0, 128, 0 };

			m_vertices.push_back(v);

			// connecting vertices as quads
			u16 up1 = i;
			u16 up2 = (i + 1) % n;
			u16 down1 = base_index + i;
			u16 down2 = base_index + ((i + 1) % n);

			if (rand() % 2)
			{
				// triangle 1
				m_indices.push_back(up1);
				m_indices.push_back(up2);
				m_indices.push_back(down1);
				// triangle 2
				m_indices.push_back(up2);
				m_indices.push_back(down2);
				m_indices.push_back(down1);
			}
			else
			{
				// triangle 1
				m_indices.push_back(up1);
				m_indices.push_back(up2);
				m_indices.push_back(down2);
				// triangle 2
				m_indices.push_back(up1);
				m_indices.push_back(down2);
				m_indices.push_back(down1);
			}
		}

		// building up the island body
		const unsigned max_level = 10;
		for (unsigned level = 0; level < max_level; ++level)
		{
			float radius = (1.f - ((float)level / (float)max_level));
			float ypos = 2.f + (level * 3.f);

			for (size_t i = 0, n = contour.size(), base_index = m_vertices.size(); i < n; ++i)
			{
				// set up new vertex
				video::S3DVertex v;
				v.Pos = m_vertices[i].Pos;// + getRandVect(1.f);
				v.Pos.X = (v.Pos.X - center.X) * radius + center.X;
				v.Pos.Z = (v.Pos.Z - center.Y) * radius + center.Y;
				v.Pos.Y -= ypos;
				if (level > 0) v.Pos += getRandVect(1.f);
				v.Normal = core::vector3df(v.Pos.X, 0.f, v.Pos.Y).normalize();
				v.Color = { 255, 32, 32, 32 };

				// add the new vertex
				m_vertices.push_back(v);

				// connecting vertices as quads
				u16 up1 = base_index - n + i;
				u16 up2 = base_index - n + ((i + 1) % n);
				u16 down1 = base_index + i;
				u16 down2 = base_index + ((i + 1) % n);

				if (rand() % 2)
				{
					// triangle 1
					m_indices.push_back(up1);
					m_indices.push_back(up2);
					m_indices.push_back(down1);
					// triangle 2
					m_indices.push_back(up2);
					m_indices.push_back(down2);
					m_indices.push_back(down1);
				}
				else
				{
					// triangle 1
					m_indices.push_back(up1);
					m_indices.push_back(up2);
					m_indices.push_back(down2);
					// triangle 2
					m_indices.push_back(up1);
					m_indices.push_back(down2);
					m_indices.push_back(down1);
				}
			}
		}

		// adding bottom point
		for (size_t i = 0, n = contour.size(), base_index = m_vertices.size(); i < n; ++i)
		{
			u16 up1 = base_index - n + i;
			u16 up2 = base_index - n + ((i + 1) % n);
			u16 bottom = base_index;

			m_indices.push_back(up1);
			m_indices.push_back(up2);
			m_indices.push_back(bottom);
		}
		video::S3DVertex v;
		v.Pos.X = center.X;
		v.Pos.Z = center.Y;
		v.Pos.Y -= 2.f + (max_level * 3.f);
		v.Normal = { 0.f, -1.f, 0.f };
		v.Color = { 255, 32, 32, 32 };
		m_vertices.push_back(v);

		//getSceneManager()->getMeshManipulator()->recalculateNormals(&m_meshbuffer);
		m_meshbuffer.recalculateBoundingBox();
		m_meshbuffer.setDirty();

		return true;
	}

	void clear()
	{
		m_vertices.set_used(0);
		m_indices.set_used(0);
		m_meshbuffer.setDirty();
	}

	virtual void OnRegisterSceneNode()
	{
		if (IsVisible)
			SceneManager->registerNodeForRendering(this);

		ISceneNode::OnRegisterSceneNode();
	}

	virtual void render()
	{
		video::IVideoDriver* driver = SceneManager->getVideoDriver();

		driver->setTransform(video::ETS_WORLD, AbsoluteTransformation);
		driver->setMaterial(m_material);
		driver->drawMeshBuffer(&m_meshbuffer);
	}

	virtual const core::aabbox3d<f32>& getBoundingBox() const
	{
		return m_meshbuffer.getBoundingBox();
	}

	virtual u32 getMaterialCount() const
	{
		return 1;
	}

	virtual video::SMaterial& getMaterial(u32 i)
	{
		return m_material;
	}
};

int main()
{
	SIrrlichtCreationParameters params; // constructor sets the defaults
	params.AntiAlias = 1;//16;
	params.Bits = 32;
	//params.DriverType = video::EDT_DIRECT3D9;
	params.DriverType = video::EDT_OPENGL;
	params.Fullscreen = false;
	params.WindowSize = core::dimension2du(1366, 768);
	params.WithAlphaChannel = true;
	params.Vsync = true;

	IrrlichtDevice* device = createDeviceEx(params);
	video::IVideoDriver* driver = device->getVideoDriver();
	scene::ISceneManager* smgr = device->getSceneManager();
	EventReceiver* evtrec = new EventReceiver();
	MouseController mctrl(smgr, evtrec);

	device->setEventReceiver(evtrec);
	device->setResizable(true);
	device->setWindowCaption(L"Ground Tool");

	auto cam = smgr->addCameraSceneNode(0, core::vector3df(10.f, 50.f, 10.f), core::vector3df(0.f, 0.f, 0.f));
	smgr->addLightSceneNode(0, { 0.f, 30.f, 30.f }, { 255, 255, 255, 255 }, 0.5f);

	core::plane3df plane(core::vector3df(0.f, 0.f, 0.f), core::vector3df(0.f, 1.f, 0.f));
	video::SMaterial line_material;
	line_material.Lighting = false;
	line_material.Thickness = 2.f;

	srand(time(NULL));

	bool quit = false;
	bool contour_finished = false;
	std::vector<core::vector2df> contour;
	Ground* ground = new Ground(smgr);

	evtrec->addKeyCallback([&](const SEvent::SKeyInput& event) -> bool
	{
		switch (event.Key)
		{
		case EKEY_CODE::KEY_ESCAPE:
			quit = true;
			return true;

		case EKEY_CODE::KEY_KEY_R:
			contour.clear();
			//indices.clear();
			ground->clear();
			contour_finished = false;
			return true;

		case EKEY_CODE::KEY_SPACE:
			cam->setPosition({ 10.f, 50.f, 10.f });
			cam->setTarget({ 0.f, 0.f, 0.f });
			return true;
		}

		return false;
	});

	evtrec->addMouseCallback([&](const SEvent::SMouseInput& event) -> bool
	{
		if (event.isLeftPressed())
		{
			core::vector3df intersection;
			core::line3df ray = smgr->getSceneCollisionManager()->getRayFromScreenCoordinates({ event.X, event.Y });
			if (plane.getIntersectionWithLine(ray.start, ray.end + ray.start, intersection))
			{
				core::vector2df point(intersection.X, intersection.Z);

				if (!contour_finished)
				{
					for (auto& p : contour)
					{
						if (point.getDistanceFrom(p) < 3.0f)
						{
							if (contour.size() > 2 && p == contour[0])
							{
								contour_finished = true;
								ground->init(contour);
								return true;
							}

							return false;
						}
					}

					contour.push_back(point);
					return true;
				}
			}
		}

		return false;
	});

	while (device->run() && !quit)
	{
		driver->beginScene(true, true, { 255, 128, 196, 255 });

		smgr->drawAll();

		driver->setMaterial(line_material);

		//if (!contour_finished)
		{
			for (auto& p : contour)
			{
				const core::vector3df edge(0.1f, 0.1f, 0.1f);

				driver->draw3DBox({ to3d(p) + edge, to3d(p) - edge }, { 255, 0, 0, 0 });

				if (p != contour.back())
				{
					driver->draw3DLine(to3d(p), to3d((&p)[1]), { 255, 0, 0, 0 });
				}
				else if (contour_finished)
				{
					driver->draw3DLine(to3d(p), to3d(contour[0]), { 255, 0, 0, 0 });
				}
			}
		}

		driver->endScene();
	}

	device->drop();

	return 0;
}
