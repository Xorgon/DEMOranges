#### import the simple module from the paraview
from paraview.simple import *
import numpy as np
import math

#### disable automatic camera reset on 'Show'
paraview.simple._DisableFirstRenderCameraReset()

# create a new 'CSV Reader'
a100000_ROTATING_BOX_ = FindSource('100000_ROTATING_BOX_*')

# get animation scene
scene = GetAnimationScene()

# update animation scene based on data timesteps
scene.UpdateAnimationUsingDataTimeSteps()

# Properties modified on a100000_ROTATING_BOX_
a100000_ROTATING_BOX_.HaveHeaders = 0

# find view
renderView1 = FindViewOrCreate('RenderView1', viewtype='RenderView')
# uncomment following to set a specific view size
# renderView1.ViewSize = [1595, 693]

# set active view
SetActiveView(renderView1)

print("Loading particles...")

# create a new 'Table To Points'
tableToPoints1 = TableToPoints(Input=a100000_ROTATING_BOX_)
tableToPoints1.XColumn = 'Field 0'
tableToPoints1.YColumn = 'Field 0'
tableToPoints1.ZColumn = 'Field 0'

# Properties modified on tableToPoints1
tableToPoints1.YColumn = 'Field 1'
tableToPoints1.ZColumn = 'Field 2'

# show data in view
tableToPoints1Display = Show(tableToPoints1, renderView1)
# trace defaults for the display properties.
tableToPoints1Display.Representation = 'Surface'
tableToPoints1Display.ColorArrayName = [None, '']
tableToPoints1Display.OSPRayScaleArray = 'Field 3'
tableToPoints1Display.OSPRayScaleFunction = 'PiecewiseFunction'
tableToPoints1Display.SelectOrientationVectors = 'Field 3'
tableToPoints1Display.ScaleFactor = 0.5548172
tableToPoints1Display.SelectScaleArray = 'Field 3'
tableToPoints1Display.GlyphType = 'Arrow'
tableToPoints1Display.GlyphTableIndexArray = 'Field 3'
tableToPoints1Display.DataAxesGrid = 'GridAxesRepresentation'
tableToPoints1Display.PolarAxes = 'PolarAxesRepresentation'
tableToPoints1Display.GaussianRadius = 0.2774086
tableToPoints1Display.SetScaleArray = ['POINTS', 'Field 3']
tableToPoints1Display.ScaleTransferFunction = 'PiecewiseFunction'
tableToPoints1Display.OpacityArray = ['POINTS', 'Field 3']
tableToPoints1Display.OpacityTransferFunction = 'PiecewiseFunction'

# reset view to fit data
renderView1.ResetCamera()

# update the view to ensure updated data information
renderView1.Update()

# set active source
SetActiveSource(a100000_ROTATING_BOX_)

# set active source
SetActiveSource(tableToPoints1)

transforms = []

# create a new 'Transform'
transform1 = Transform(Input=tableToPoints1)
transform1.Transform = 'Transform'

transforms.append(transform1)

# toggle 3D widget visibility (only when running from the GUI)
Hide3DWidgets(proxy=transform1.Transform)

# show data in view
transform1Display = Show(transform1, renderView1)
# trace defaults for the display properties.
transform1Display.Representation = 'Surface'
transform1Display.ColorArrayName = [None, '']
transform1Display.OSPRayScaleArray = 'Field 3'
transform1Display.OSPRayScaleFunction = 'PiecewiseFunction'
transform1Display.SelectOrientationVectors = 'Field 3'
transform1Display.ScaleFactor = 0.5548172
transform1Display.SelectScaleArray = 'Field 3'
transform1Display.GlyphType = 'Arrow'
transform1Display.GlyphTableIndexArray = 'Field 3'
transform1Display.DataAxesGrid = 'GridAxesRepresentation'
transform1Display.PolarAxes = 'PolarAxesRepresentation'
transform1Display.GaussianRadius = 0.2774086
transform1Display.SetScaleArray = ['POINTS', 'Field 3']
transform1Display.ScaleTransferFunction = 'PiecewiseFunction'
transform1Display.OpacityArray = ['POINTS', 'Field 3']
transform1Display.OpacityTransferFunction = 'PiecewiseFunction'

# hide data in view
Hide(tableToPoints1, renderView1)

# update the view to ensure updated data information
renderView1.Update()

# create a new 'Glyph'
glyph1 = Glyph(Input=transform1,
               GlyphType='Sphere')
glyph1.Scalars = ['POINTS', 'Field 3']
glyph1.Vectors = ['POINTS', 'None']
glyph1.GlyphTransform = 'Transform2'

# Properties modified on glyph1.GlyphType
glyph1.GlyphType.Radius = 0.05

# Properties modified on glyph1
glyph1.GlyphType = 'Sphere'
glyph1.Scalars = ['POINTS', 'None']
glyph1.ScaleFactor = 1.0

# Properties modified on glyph1.GlyphType
glyph1.GlyphType.Radius = 0.05

# show data in view
glyph1Display = Show(glyph1, renderView1)

# update the view to ensure updated data information
renderView1.Update()

# Properties modified on glyph1
glyph1.GlyphMode = 'All Points'

# update the view to ensure updated data information
renderView1.Update()

# hide data in view
Hide(transform1, renderView1)

print("Generating box...")
box_length = 7.05

# create a new 'Plane'
plane1 = Plane()

# Properties modified on plane1
plane1.Origin = [-0.5, -0.5, -0.5]
plane1.Point1 = [-0.5, -0.5, 0.5]
plane1.Point2 = [0.5, -0.5, -0.5]

transformP1 = Transform(Input=plane1)
transformP1.Transform = 'Transform'
transformP1.GetProperty('Transform').GetData().SetPropertyWithName('Scale', [box_length, box_length, box_length])
transforms.append(transformP1)

# create a new 'Plane'
plane2 = Plane()

# Properties modified on plane2
plane2.Origin = [-0.5, 0.5, -0.5]
plane2.Point1 = [-0.5, 0.5, 0.5]
plane2.Point2 = [0.5, 0.5, -0.5]

transformP2 = Transform(Input=plane2)
transformP2.Transform = 'Transform'
transformP2.GetProperty('Transform').GetData().SetPropertyWithName('Scale', [box_length, box_length, box_length])
transforms.append(transformP2)

# create a new 'Plane'
plane3 = Plane()

# Properties modified on plane3
plane3.Origin = [-0.5, -0.5, -0.5]
plane3.Point1 = [-0.5, -0.5, 0.5]
plane3.Point2 = [-0.5, 0.5, -0.5]

transformP3 = Transform(Input=plane3)
transformP3.Transform = 'Transform'
transformP3.GetProperty('Transform').GetData().SetPropertyWithName('Scale', [box_length, box_length, box_length])
transforms.append(transformP3)

# create a new 'Plane'
plane4 = Plane()

# Properties modified on plane4
plane4.Origin = [0.5, -0.5, -0.5]
plane4.Point1 = [0.5, -0.5, 0.5]
plane4.Point2 = [0.5, 0.5, -0.5]

transformP4 = Transform(Input=plane4)
transformP4.Transform = 'Transform'
transformP4.GetProperty('Transform').GetData().SetPropertyWithName('Scale', [box_length, box_length, box_length])
transforms.append(transformP4)

# create a new 'Plane'
plane5 = Plane()

# Properties modified on plane5
plane5.Origin = [-0.5, -0.5, -0.5]
plane5.Point1 = [0.5, -0.5, -0.5]
plane5.Point2 = [-0.5, 0.5, -0.5]

transformP5 = Transform(Input=plane5)
transformP5.Transform = 'Transform'
transformP5.GetProperty('Transform').GetData().SetPropertyWithName('Scale', [box_length, box_length, box_length])
transforms.append(transformP5)

# create a new 'Plane'
plane6 = Plane()

# Properties modified on plane6
plane6.Origin = [-0.5, -0.5, 0.5]
plane6.Point1 = [0.5, -0.5, 0.5]
plane6.Point2 = [-0.5, 0.5, 0.5]

transformP6 = Transform(Input=plane6)
transformP6.Transform = 'Transform'
transformP6.GetProperty('Transform').GetData().SetPropertyWithName('Scale', [box_length, box_length, box_length])
transforms.append(transformP6)

renderView1 = GetActiveViewOrCreate('RenderView')
plane1Display = Hide(plane1, renderView1)
plane2Display = Hide(plane2, renderView1)
plane3Display = Hide(plane3, renderView1)
plane4Display = Hide(plane4, renderView1)
plane5Display = Hide(plane5, renderView1)
plane6Display = Hide(plane6, renderView1)

transformP1Display = Show(transformP1, renderView1)
transformP2Display = Show(transformP2, renderView1)
transformP3Display = Show(transformP3, renderView1)
transformP4Display = Show(transformP4, renderView1)
transformP5Display = Show(transformP5, renderView1)
transformP6Display = Show(transformP6, renderView1)

# # Properties modified on plane1Display
# plane1Display.Scale = [box_length, box_length, box_length]
# plane1Display.DataAxesGrid.Scale = [box_length, box_length, box_length]
# plane1Display.PolarAxes.Scale = [box_length, box_length, box_length]
# 
# # Properties modified on plane2Display
# plane2Display.Scale = [box_length, box_length, box_length]
# plane2Display.DataAxesGrid.Scale = [box_length, box_length, box_length]
# plane2Display.PolarAxes.Scale = [box_length, box_length, box_length]
# 
# # Properties modified on plane3Display
# plane3Display.Scale = [box_length, box_length, box_length]
# plane3Display.DataAxesGrid.Scale = [box_length, box_length, box_length]
# plane3Display.PolarAxes.Scale = [box_length, box_length, box_length]
# 
# # Properties modified on plane4Display
# plane4Display.Scale = [box_length, box_length, box_length]
# plane4Display.DataAxesGrid.Scale = [box_length, box_length, box_length]
# plane4Display.PolarAxes.Scale = [box_length, box_length, box_length]
# 
# # Properties modified on plane5Display
# plane5Display.Scale = [box_length, box_length, box_length]
# plane5Display.DataAxesGrid.Scale = [box_length, box_length, box_length]
# plane5Display.PolarAxes.Scale = [box_length, box_length, box_length]
# 
# # Properties modified on plane6Display
# plane6Display.Scale = [box_length, box_length, box_length]
# plane6Display.DataAxesGrid.Scale = [box_length, box_length, box_length]
# plane6Display.PolarAxes.Scale = [box_length, box_length, box_length]

# hide data in view
Hide(transformP6, renderView1)

# reset view to fit data bounds
renderView1.ResetCamera(-3.525, 3.525, -3.525, 3.525, 3.525, 3.525)

# Hide orientation axes
renderView1.OrientationAxesVisibility = 0

# Properties modified on renderView1
renderView1.SamplesPerPixel = 4

# Properties modified on renderView1
# renderView1.Shadows = 1
# renderView1.EnableOSPRay = 1

# Properties modified on renderView1
renderView1.Background = [0.0, 0.0, 0.0]

# current camera placement for renderView1
renderView1.CameraPosition = [0.0, 0.0, 22.785958193360585]
renderView1.CameraFocalPoint = [0.0, 0.0, 3.525]
renderView1.CameraParallelScale = 4.98510280736516

max_rot = np.sqrt(16 * 2 * 3.14159265358979 / 0.05)


def get_rotation(t):
    if t > max_rot:
        t_fact = 0
    else:
        t_fact = 0.05 * t * t
    return math.degrees(t_fact)


print("Creating animation...")
cues = []
for transform in transforms:
    cue = GetAnimationTrack(transform.GetProperty('Transform').GetData().GetProperty('Rotate'), 2, transform)
    cue.KeyFrames = []
    cues.append(cue)

key_frames = []
for t in range(0, 1798 + 1):
    angle = get_rotation(float(t) / 30.0)
    print("        %i - %.3f" % (t, angle))

    keyf = CompositeKeyFrame()
    keyf.KeyTime = t / 1798.0
    keyf.KeyValues = [-angle]
    key_frames.append(keyf)

for cue in cues:
    cue.KeyFrames = key_frames

#### uncomment the following to render all views
RenderAllViews()
# alternatively, if you want to write images, you can use SaveScreenshot(...).
