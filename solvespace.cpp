#include "solvespace.h"

template IdList<Request,hRequest>;
template IdList<Entity,hEntity>;
template IdList<Point,hPoint>;

SolveSpace SS;

void SolveSpace::Init(void) {
    TW.Init();
    GW.Init();

    request.Clear();
    entity.Clear();
    point.Clear();
    param.Clear();
    group.Clear();

    // Our initial group, that contains the references.
    Group g;
    memset(&g, 0, sizeof(g));
    g.csys = Entity::NO_CSYS;
    g.name.strcpy("#references");
    g.h = Group::HGROUP_REFERENCES;
    group.Add(&g);

    g.csys = Entity::NO_CSYS;
    g.name.strcpy("");
    group.AddAndAssignId(&g);
    

    // Let's create three two-d coordinate systems, for the coordinate
    // planes; these are our references, present in every sketch.
    Request r;
    memset(&r, 0, sizeof(r));
    r.type = Request::CSYS_2D;
    r.group = Group::HGROUP_REFERENCES;

    r.name.strcpy("#XY-csys");
    r.h = Request::HREQUEST_REFERENCE_XY;
    request.Add(&r);

    r.name.strcpy("#YZ-csys");
    r.h = Request::HREQUEST_REFERENCE_YZ;
    request.Add(&r);

    r.name.strcpy("#ZX-csys");
    r.h = Request::HREQUEST_REFERENCE_ZX;
    request.Add(&r);

    TW.Show();
    GenerateForUserInterface();
}

void SolveSpace::GenerateForUserInterface(void) {
    int i;

    entity.Clear();
    param.Clear();
    point.Clear();
    for(i = 0; i < request.elems; i++) {
        request.elem[i].t.Generate(&entity, &point, &param);
    }

    ForceReferences();
}

void SolveSpace::ForceReferences(void) {
    // Force the values of the paramters that define the three reference
    // coordinate systems.
    static const struct {
        hRequest hr;
        double a, b, c, d;
    } Quat[] = {
        { Request::HREQUEST_REFERENCE_XY, 1,    0,    0,    0, },
        { Request::HREQUEST_REFERENCE_YZ, 0.5, -0.5, -0.5, -0.5, },
        { Request::HREQUEST_REFERENCE_ZX, 0.5,  0.5,  0.5,  0.5, },
    };
    for(int i = 0; i < 3; i++) {
        hEntity he;
        he = request.FindById(Quat[i].hr)->entity(0);
        Entity *e = entity.FindById(he);
        // The origin for our coordinate system, always zero
        Vector v = Vector::MakeFrom(0, 0, 0);
        point.FindById(e->point(16))->ForceTo(v);
        // The quaternion that defines the rotation, from the table.
        param.FindById(e->param(0))->ForceTo(Quat[i].a);
        param.FindById(e->param(1))->ForceTo(Quat[i].b);
        param.FindById(e->param(2))->ForceTo(Quat[i].c);
        param.FindById(e->param(3))->ForceTo(Quat[i].d);
    }
}

void SolveSpace::Solve(void) {
}

