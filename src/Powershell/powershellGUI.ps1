# Example 1, http://www.drdobbs.com/windows/building-gui-applications-in-powershell/240049898

Add-Type -AssemblyName System.Windows.Forms
$form = New-Object Windows.Forms.Form
$form.Size = New-Object Drawing.Size @(200,100)
$form.StartPosition = "CenterScreen"
$btn = New-Object System.Windows.Forms.Button
$btn.add_click({Get-Date|Out-Host})
$btn.Text = "Click here"
$form.Controls.Add($btn)
$drc = $form.ShowDialog()



# Example 2, http://www.techotopia.com/index.php/Drawing_Graphics_using_PowerShell_1.0_and_GDI%2B

#Load the GDI+ and WinForms Assemblies
[reflection.assembly]::LoadWithPartialName( "System.Windows.Forms")
[reflection.assembly]::LoadWithPartialName( "System.Drawing")

# Create pen and brush objects 
$myBrush = new-object Drawing.SolidBrush green
$mypen = new-object Drawing.Pen black

# Create a Rectangle object for use when drawing rectangle
$rect = new-object Drawing.Rectangle 10, 10, 180, 180

# Create a Form
$form = New-Object Windows.Forms.Form

# Get the form's graphics object
$formGraphics = $form.createGraphics()


# Define the paint handler
$form.add_paint(
{

$formGraphics.FillEllipse($myBrush, $rect) # draw an ellipse using rectangle object

$mypen.color = "red" # Set the pen color
$mypen.width = 5     # ste the pen line width

$formGraphics.DrawLine($mypen, 10, 10, 190, 190) # draw a line

$formGraphics.DrawLine($mypen, 190, 10, 10, 190) # draw a line

}
)

$form.ShowDialog()   # display the dialog
