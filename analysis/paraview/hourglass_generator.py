#### import the simple module from the paraview
from paraview.simple import *
#### disable automatic camera reset on 'Show'
paraview.simple._DisableFirstRenderCameraReset()

box_length = input("box_length = ")
offset = input("offset = ")
gap_offset = input("gap_offset = ")

# create a new 'Plane'
plane1 = Plane()

# Properties modified on plane1
plane1.Origin = [-0.5, -0.5, -0.5]
plane1.Point1 = [-0.5, -0.5, 0.5]
plane1.Point2 = [0.5, -0.5, -0.5]

# create a new 'Plane'
plane2 = Plane()

# Properties modified on plane2
plane2.Origin = [-0.5, 0.5, -0.5]
plane2.Point1 = [-0.5, 0.5, 0.5]
plane2.Point2 = [0.5, 0.5, -0.5]

# create a new 'Plane'
plane3 = Plane()

# Properties modified on plane3
plane3.Origin = [-0.5, -0.5, -0.5]
plane3.Point1 = [-0.5, -0.5, 0.5]
plane3.Point2 = [-0.5, 0.5, -0.5]

# create a new 'Plane'
plane4 = Plane()

# Properties modified on plane4
plane4.Origin = [0.5, -0.5, -0.5]
plane4.Point1 = [0.5, -0.5, 0.5]
plane4.Point2 = [0.5, 0.5, -0.5]

# create a new 'Plane'
plane5 = Plane()

# Properties modified on plane5
plane5.Origin = [-0.5, -0.5, -0.5]
plane5.Point1 = [0.5, -0.5, -0.5]
plane5.Point2 = [-0.5, 0.5, -0.5]

# create a new 'Plane'
plane6 = Plane()

# Properties modified on plane6
plane6.Origin = [-0.5, -0.5, 0.5]
plane6.Point1 = [0.5, -0.5, 0.5]
plane6.Point2 = [-0.5, 0.5, 0.5]

renderView1 = GetActiveViewOrCreate('RenderView')
plane1Display = Show(plane1, renderView1)
plane2Display = Show(plane2, renderView1)
plane3Display = Show(plane3, renderView1)
plane4Display = Show(plane4, renderView1)
plane5Display = Show(plane5, renderView1)
plane6Display = Show(plane6, renderView1)

# Properties modified on plane6Display
plane6Display.Opacity = 0.5

# Properties modified on plane1Display
plane1Display.Scale = [box_length, box_length, box_length]
plane1Display.DataAxesGrid.Scale = [box_length, box_length, box_length]
plane1Display.PolarAxes.Scale = [box_length, box_length, box_length]

# Properties modified on plane2Display
plane2Display.Scale = [box_length, box_length, box_length]
plane2Display.DataAxesGrid.Scale = [box_length, box_length, box_length]
plane2Display.PolarAxes.Scale = [box_length, box_length, box_length]

# Properties modified on plane3Display
plane3Display.Scale = [box_length, box_length, box_length]
plane3Display.DataAxesGrid.Scale = [box_length, box_length, box_length]
plane3Display.PolarAxes.Scale = [box_length, box_length, box_length]

# Properties modified on plane4Display
plane4Display.Scale = [box_length, box_length, box_length]
plane4Display.DataAxesGrid.Scale = [box_length, box_length, box_length]
plane4Display.PolarAxes.Scale = [box_length, box_length, box_length]

# Properties modified on plane5Display
plane5Display.Scale = [box_length, box_length, box_length]
plane5Display.DataAxesGrid.Scale = [box_length, box_length, box_length]
plane5Display.PolarAxes.Scale = [box_length, box_length, box_length]

# Properties modified on plane6Display
plane6Display.Scale = [box_length, box_length, box_length]
plane6Display.DataAxesGrid.Scale = [box_length, box_length, box_length]
plane6Display.PolarAxes.Scale = [box_length, box_length, box_length]


# Properties modified on plane1Display
plane1Display.Position = [offset, offset, 0.0]
plane1Display.DataAxesGrid.Position = [offset, offset, 0.0]
plane1Display.PolarAxes.Translation = [offset, offset, 0.0]

# Properties modified on plane2Display
plane2Display.Position = [offset, offset, 0.0]
plane2Display.DataAxesGrid.Position = [offset, offset, 0.0]
plane2Display.PolarAxes.Translation = [offset, offset, 0.0]


# Properties modified on plane3Display
plane3Display.Position = [offset, offset, 0.0]
plane3Display.DataAxesGrid.Position = [offset, offset, 0.0]
plane3Display.PolarAxes.Translation = [offset, offset, 0.0]

# Properties modified on plane4Display
plane4Display.Position = [offset, offset, 0.0]
plane4Display.DataAxesGrid.Position = [offset, offset, 0.0]
plane4Display.PolarAxes.Translation = [offset, offset, 0.0]

# Properties modified on plane5Display
plane5Display.Position = [offset, offset, 0.0]
plane5Display.DataAxesGrid.Position = [offset, offset, 0.0]
plane5Display.PolarAxes.Translation = [offset, offset, 0.0]

# Properties modified on plane6Display
plane6Display.Position = [offset, offset, 0.0]
plane6Display.DataAxesGrid.Position = [offset, offset, 0.0]
plane6Display.PolarAxes.Translation = [offset, offset, 0.0]

# Properties modified on plane1
plane1.Origin = [-gap_offset, -0.5, -0.5]
plane1.Point1 = [-gap_offset, -0.5, 0.5]

# Properties modified on plane3
plane3.Origin = [-0.5, -gap_offset, -0.5]
plane3.Point1 = [-0.5, -gap_offset, 0.5]

# update the view to ensure updated data information
renderView1.Update()

###################
### SECOND CUBE ###
###################

# create a new 'Plane'
plane7 = Plane()

# Properties modified on plane7
plane7.Origin = [-0.5, -0.5, -0.5]
plane7.Point1 = [-0.5, -0.5, 0.5]
plane7.Point2 = [0.5, -0.5, -0.5]

# create a new 'Plane'
plane8 = Plane()

# Properties modified on plane8
plane8.Origin = [-0.5, 0.5, -0.5]
plane8.Point1 = [-0.5, 0.5, 0.5]
plane8.Point2 = [0.5, 0.5, -0.5]

# create a new 'Plane'
plane9 = Plane()

# Properties modified on plane9
plane9.Origin = [-0.5, -0.5, -0.5]
plane9.Point1 = [-0.5, -0.5, 0.5]
plane9.Point2 = [-0.5, 0.5, -0.5]

# create a new 'Plane'
plane10 = Plane()

# Properties modified on plane10
plane10.Origin = [0.5, -0.5, -0.5]
plane10.Point1 = [0.5, -0.5, 0.5]
plane10.Point2 = [0.5, 0.5, -0.5]

# create a new 'Plane'
plane11 = Plane()

# Properties modified on plane11
plane11.Origin = [-0.5, -0.5, -0.5]
plane11.Point1 = [0.5, -0.5, -0.5]
plane11.Point2 = [-0.5, 0.5, -0.5]

# create a new 'Plane'
plane12 = Plane()

# Properties modified on plane12
plane12.Origin = [-0.5, -0.5, 0.5]
plane12.Point1 = [0.5, -0.5, 0.5]
plane12.Point2 = [-0.5, 0.5, 0.5]

renderView1 = GetActiveViewOrCreate('RenderView')
plane7Display = Show(plane7, renderView1)
plane8Display = Show(plane8, renderView1)
plane9Display = Show(plane9, renderView1)
plane10Display = Show(plane10, renderView1)
plane11Display = Show(plane11, renderView1)
plane12Display = Show(plane12, renderView1)

# Properties modified on plane12Display
plane12Display.Opacity = 0.5

# Properties modified on plane7Display
plane7Display.Scale = [box_length, box_length, box_length]
plane7Display.DataAxesGrid.Scale = [box_length, box_length, box_length]
plane7Display.PolarAxes.Scale = [box_length, box_length, box_length]

# Properties modified on plane8Display
plane8Display.Scale = [box_length, box_length, box_length]
plane8Display.DataAxesGrid.Scale = [box_length, box_length, box_length]
plane8Display.PolarAxes.Scale = [box_length, box_length, box_length]

# Properties modified on plane9Display
plane9Display.Scale = [box_length, box_length, box_length]
plane9Display.DataAxesGrid.Scale = [box_length, box_length, box_length]
plane9Display.PolarAxes.Scale = [box_length, box_length, box_length]

# Properties modified on plane10Display
plane10Display.Scale = [box_length, box_length, box_length]
plane10Display.DataAxesGrid.Scale = [box_length, box_length, box_length]
plane10Display.PolarAxes.Scale = [box_length, box_length, box_length]

# Properties modified on plane11Display
plane11Display.Scale = [box_length, box_length, box_length]
plane11Display.DataAxesGrid.Scale = [box_length, box_length, box_length]
plane11Display.PolarAxes.Scale = [box_length, box_length, box_length]

# Properties modified on plane12Display
plane12Display.Scale = [box_length, box_length, box_length]
plane12Display.DataAxesGrid.Scale = [box_length, box_length, box_length]
plane12Display.PolarAxes.Scale = [box_length, box_length, box_length]

# Properties modified on plane7Display
plane7Display.Position = [-offset, -offset, 0.0]
plane7Display.DataAxesGrid.Position = [-offset, -offset, 0.0]
plane7Display.PolarAxes.Translation = [-offset, -offset, 0.0]

# Properties modified on plane8Display
plane8Display.Position = [-offset, -offset, 0.0]
plane8Display.DataAxesGrid.Position = [-offset, -offset, 0.0]
plane8Display.PolarAxes.Translation = [-offset, -offset, 0.0]

# Properties modified on plane9Display
plane9Display.Position = [-offset, -offset, 0.0]
plane9Display.DataAxesGrid.Position = [-offset, -offset, 0.0]
plane9Display.PolarAxes.Translation = [-offset, -offset, 0.0]

# Properties modified on plane10Display
plane10Display.Position = [-offset, -offset, 0.0]
plane10Display.DataAxesGrid.Position = [-offset, -offset, 0.0]
plane10Display.PolarAxes.Translation = [-offset, -offset, 0.0]

# Properties modified on plane11Display
plane11Display.Position = [-offset, -offset, 0.0]
plane11Display.DataAxesGrid.Position = [-offset, -offset, 0.0]
plane11Display.PolarAxes.Translation = [-offset, -offset, 0.0]

# Properties modified on plane12Display
plane12Display.Position = [-offset, -offset, 0.0]
plane12Display.DataAxesGrid.Position = [-offset, -offset, 0.0]
plane12Display.PolarAxes.Translation = [-offset, -offset, 0.0]

# Properties modified on plane8
plane8.Point2 = [gap_offset, 0.5, -0.5]

# Properties modified on plane10
plane10.Point2 = [0.5, gap_offset, -0.5]

# update the view to ensure updated data information
renderView1.Update()