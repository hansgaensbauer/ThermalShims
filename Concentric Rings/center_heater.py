import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
from matplotlib.patches import Ellipse

class Coil:
    def __init__(self, points):
        self.points = points  # Shape: (N, 3)

    @classmethod
    def circular(cls, radius=1.0, turns=1, points_per_turn=100, center=(0, 0, 0), axis='z'):
        total_points = turns * points_per_turn
        theta = np.linspace(0, 2 * np.pi * turns, total_points)
        x = radius * np.cos(theta)
        y = radius * np.sin(theta)
        z = np.zeros_like(theta)

        if axis == 'x':
            points = np.vstack((z, x, y)).T
        elif axis == 'y':
            points = np.vstack((x, z, y)).T
        else:
            points = np.vstack((x, y, z)).T

        points += np.array(center)
        return cls(points)
    
    @classmethod
    def spiral(cls, radius=1.0, turns=1, pitch=0.001, points_per_turn=100, center=(0, 0.2, 0), axis='z'):
        total_points = turns * points_per_turn
        theta = np.linspace(0, 2 * np.pi * turns, total_points)
        x = np.cos(-theta)*(theta+radius/pitch * 2 * np.pi)/(2*np.pi)*pitch
        y = np.sin(-theta)*(theta+radius/pitch * 2 * np.pi)/(2*np.pi)*pitch
        z = np.zeros_like(theta)

        if axis == 'x':
            points = np.vstack((z, x, y)).T
        elif axis == 'y':
            points = np.vstack((x, z, y)).T
        else:
            points = np.vstack((x, y, z)).T

        points += np.array(center)
        return cls(points)

    @classmethod
    def boxspiral(cls, height, width, length, turns=10, points_per_meter=10000, center=(0,0,0), axis='z'):
        riser_points = int(height * points_per_meter)
        horizontal_points = int(width * points_per_meter)
        diagonal_points = int(np.sqrt((length/turns)**2 + width**2)*points_per_meter)
        points_per_turn = 2 * riser_points + horizontal_points + diagonal_points
        total_points = turns * points_per_turn
        x = np.empty(total_points)
        y = np.empty(total_points)
        z = np.empty(total_points)

        for i in range(turns):
            turn_start_idx = i*points_per_turn
            #First vertical segment
            x[turn_start_idx:turn_start_idx + riser_points] = np.zeros(riser_points) - length/2 + (length/turns*i)
            y[turn_start_idx:turn_start_idx + riser_points] = np.zeros(riser_points) - width/2
            z[turn_start_idx:turn_start_idx + riser_points] = np.linspace(-height/2, height/2, riser_points)
            #Horizontal segment
            x[turn_start_idx + riser_points:turn_start_idx + riser_points + horizontal_points] = np.zeros(horizontal_points) - length/2 + (length/turns*i)
            y[turn_start_idx + riser_points:turn_start_idx + riser_points + horizontal_points] = np.linspace(-width/2, width/2, horizontal_points)
            z[turn_start_idx + riser_points:turn_start_idx + riser_points + horizontal_points] = np.zeros(horizontal_points) + height/2
            #Second vertical segment
            x[turn_start_idx + riser_points + horizontal_points:turn_start_idx + 2*riser_points + horizontal_points] = np.zeros(riser_points) - length/2 + (length/turns*i)
            y[turn_start_idx + riser_points + horizontal_points:turn_start_idx + 2*riser_points + horizontal_points] = np.zeros(riser_points) + width/2
            z[turn_start_idx + riser_points + horizontal_points:turn_start_idx + 2*riser_points + horizontal_points] = np.linspace(height/2, -height/2, riser_points)
            #Diagonal segment
            x[turn_start_idx + 2*riser_points + horizontal_points:turn_start_idx + points_per_turn] = np.linspace(-length/2 + (length/turns*i), -length/2 + (length/turns*(i+1)), diagonal_points)
            y[turn_start_idx + 2*riser_points + horizontal_points:turn_start_idx + points_per_turn] = np.linspace(width/2, -width/2, diagonal_points)
            z[turn_start_idx + 2*riser_points + horizontal_points:turn_start_idx + points_per_turn] = np.zeros(diagonal_points) - height/2
        
        if axis == 'x':
            points = np.vstack((z, x, y)).T
        elif axis == 'y':
            points = np.vstack((x, z, y)).T
        else:
            points = np.vstack((x, y, z)).T

        points += np.array(center)
        return cls(points)
    
    @classmethod
    def d(cls, radius=1.0, turns=1, points_per_turn=100, center=(0, 0, 0), axis='z'):
        total_points = turns * points_per_turn
        theta = np.linspace(0, 2 * np.pi * turns, total_points)
        x_firsthalf = radius * np.cos(theta[:int(points_per_turn/2)])
        y_firsthalf = radius * np.sin(theta[:int(points_per_turn/2)])
        z_firsthalf = np.zeros_like(y_firsthalf)
        x_cross = np.linspace(-radius, radius, int(points_per_turn/np.pi))
        y_cross = np.zeros_like(x_cross)
        z_cross = np.zeros_like(x_cross)
        x_secondhalf = -radius * np.cos(theta[int(points_per_turn/2):])
        y_secondhalf = radius * np.sin(theta[int(points_per_turn/2):])
        z_secondhalf = np.zeros_like(y_secondhalf)
        x_returncross = np.linspace(-radius, radius, int(points_per_turn/np.pi))
        y_returncross = np.zeros_like(x_cross)
        z_returncross = np.zeros_like(x_cross)

        x = np.concatenate((x_firsthalf, x_cross, x_secondhalf, x_returncross))
        y = np.concatenate((y_firsthalf, y_cross, y_secondhalf, y_returncross))
        z = np.concatenate((z_firsthalf, z_cross, z_secondhalf, z_returncross))

        if axis == 'x':
            points = np.vstack((z, x, y)).T
        elif axis == 'y':
            points = np.vstack((x, z, y)).T
        else:
            points = np.vstack((y, x, z)).T #WARNING! This is swapped

        points += np.array(center)
        return cls(points)

    def get_segments(self):
        segments = np.stack((self.points[:-1], self.points[1:]), axis=1)
        return segments

    def get_current_elements(self, I=1.0):
        segments = self.get_segments()
        dl = segments[:, 1] - segments[:, 0]
        r = (segments[:, 0] + segments[:, 1]) / 2
        return dl, r, I
    
    def get_length(self):
        segments = self.get_segments()
        dl = segments[:, 1] - segments[:, 0]
        return np.sum(np.linalg.norm(dl, axis=1))

    def plot(self, show=True, ax=None, **kwargs):
        def _set_axes_equal(ax):
            """
            Make axes of 3D plot have equal scale so that spheres appear as spheres,
            cubes as cubes, etc.

            Input
            ax: a matplotlib axis, e.g., as output from plt.gca().
            """

            x_limits = ax.get_xlim3d()
            y_limits = ax.get_ylim3d()
            z_limits = ax.get_zlim3d()

            x_range = abs(x_limits[1] - x_limits[0])
            x_middle = np.mean(x_limits)
            y_range = abs(y_limits[1] - y_limits[0])
            y_middle = np.mean(y_limits)
            z_range = abs(z_limits[1] - z_limits[0])
            z_middle = np.mean(z_limits)

            # The plot bounding box is a sphere in the sense of the infinity
            # norm, hence I call half the max range the plot radius.
            plot_radius = 0.5*max([x_range, y_range, z_range])

            ax.set_xlim3d([x_middle - plot_radius, x_middle + plot_radius])
            ax.set_ylim3d([y_middle - plot_radius, y_middle + plot_radius])
            ax.set_zlim3d([z_middle - plot_radius, z_middle + plot_radius])

        if ax is None:
            fig = plt.figure()
            ax = fig.add_subplot(111, projection='3d')

        ax.plot(self.points[:, 0], self.points[:, 1], self.points[:, 2], '.', **kwargs)
        ax.set_xlabel('X')
        ax.set_ylabel('Y')
        ax.set_zlabel('Z')

        if show:
            if ax is None:
                _set_axes_equal(ax)
                ax.set_box_aspect([1.0, 1.0, 1.0])
            plt.show()

    def biot_savart(self, points, I=1.0, mu_0=4*np.pi*1e-7):
        def _biot_savart(observation_point, I):
            dl, r, current = self.get_current_elements(I)
            R = observation_point - r
            norm_R = np.linalg.norm(R, axis=1).reshape(-1, 1)
            cross = np.cross(dl, R)
            dB = (mu_0 / (4 * np.pi)) * (current * cross) / (norm_R**3)
            B = np.sum(dB, axis=0)
            return B
        
        Bx, By, Bz = np.zeros_like(points[:,0]), np.zeros_like(points[:,1]), np.zeros_like(points[:,2])

        for idx, point in enumerate(points):
            B = _biot_savart(point, I=I)
            Bx[idx], By[idx], Bz[idx] = B

        return Bx, By, Bz

class ROI:    
    def __init__(self, radius, top, bottom, point_density=8000, m=[1,0,0]):
        self.radius = radius
        self.top = top
        self.bottom = bottom
        self.m = m

        #points within the ROI
        x = np.linspace(-radius, radius, int(2*radius*point_density))
        y = np.linspace(-radius, radius, int(2*radius*point_density))
        z = np.linspace(bottom, top, int((top-bottom)*point_density))
        X, Y, Z = np.meshgrid(x, y, z)
        R = np.sqrt(X**2 + Y**2)

        # Create a mask for points inside the cylindrical volume (R <= radius)
        inside_cylinder = np.logical_and((R <= self.radius), np.abs(Z - (self.top + self.bottom)/2) < (self.top - self.bottom)/2)

        # Apply the mask to filter out points outside the cylinder
        X = np.where(inside_cylinder, X, np.nan)
        Y = np.where(inside_cylinder, Y, np.nan)
        Z = np.where(inside_cylinder, Z, np.nan)

        points = np.vstack((X.ravel(), Y.ravel(), Z.ravel())).T
        self.points = points[~np.isnan(np.sum(points, axis=1))]
    
    def integrate(self, coil):
        Bx,By,Bz = coil.biot_savart(self.points)
        return np.sum(Bx)
    
    def magnetization(self, observation_points, mu_0=4*np.pi*1e-7):
        def _magnetization(observation_point, mu_0):
            R = observation_point - self.points
            norm_R = np.linalg.norm(R, axis=1).reshape(-1, 1)
            B = mu_0/(4 * np.pi) * ((3*R * np.tile(np.dot(self.m, R.T),3).reshape(-1,3))/norm_R**5 - self.m/norm_R**3)
            return np.sum(B,axis=0)
        
        B = np.zeros((len(observation_points),3))
        for idx, point in enumerate(observation_points):
            B_pt = _magnetization(point, mu_0/len(self.points)/1000000)
            B[idx] = B_pt

        return B[:,0], B[:,1], B[:,2]
    
    def plot(self, ax=None, show=True, **kwargs):
        if ax is None:
            fig = plt.figure()
            ax = fig.add_subplot(111, projection='3d')
        resolution = 100
            # Generate data for the cylinder
        z = np.linspace(self.bottom, self.top, resolution)  # Height values
        theta = np.linspace(0, 2 * np.pi, resolution)  # Angle values
        theta, z = np.meshgrid(theta, z)  # Create a 2D grid for theta and z

        # Parametric equations for the cylinder
        x = self.radius * np.cos(theta)
        y = self.radius * np.sin(theta)

        # Plot the surface of the cylinder
        ax.plot_surface(x, y, z, color='cyan', alpha=0.7, rstride=5, cstride=5)
        # ax.scatter(self.points[:,0],self.points[:,1],self.points[:,2])

        if(show):
            # Customize the plot
            ax.set_xlabel('X')
            ax.set_ylabel('Y')
            ax.set_zlabel('Z')
            ax.set_title('3D Cylinder')
            axis_size = 1.5*np.maximum(self.radius, (self.top-self.bottom)/2)
            ax.set_xlim([-axis_size, axis_size])
            ax.set_ylim([-axis_size, axis_size])
            ax.set_zlim([-axis_size, axis_size])
            plt.show()

def plot_magnetic_field(points, Bx, By, Bz, ax=None, show=False):
    if ax is None:
        fig = plt.figure()
        ax = fig.add_subplot(111, projection='3d')

    arrowscale = 0.001
    ax.quiver(points[:,0], points[:,1], points[:,2], Bx*arrowscale, By*arrowscale, Bz*arrowscale, linewidth=0.5)

    if(show): 
        # coil.plot(show=False, ax=ax, color='b', linewidth=2)
        # axis_size = 1.5*np.maximum(np.maximum(), (self.top-self.bottom)/2)
        # ax.set_xlim([-grid_size, grid_size])
        # ax.set_ylim([-grid_size, grid_size])
        # ax.set_zlim([-grid_size, grid_size])
        ax.set_xlabel('X')
        ax.set_ylabel('Y')
        ax.set_zlabel('Z')
        plt.title('Magnetic Field from Coil')

def plotcoilsim(coil, roi, points = 10, ax = None, show=True):
    if(show):
        fig = plt.figure(figsize=(8, 6))
        ax = fig.add_subplot(111, projection='3d')

    grid_size = 0.005
    x = np.linspace(-grid_size/1, grid_size/1, int(points/1))
    y = np.linspace(-grid_size/5, grid_size/5, int(points/5))
    z = np.linspace(-grid_size, grid_size, int(points))
    X, Y, Z = np.meshgrid(x, y, z)
    Bx, By, Bz = np.zeros_like(X), np.zeros_like(Y), np.zeros_like(Z)

    observation_points = np.vstack((X.ravel(), Y.ravel(), Z.ravel())).T

    Bx,By,Bz = coil.biot_savart(observation_points, 1)
    coil.plot(color='b', linewidth=1.5, ax=ax,show=False)
    roi.plot(ax, show=False)
    plot_magnetic_field(observation_points, Bx, By, Bz, ax=ax, show=False)
    ax.set_xlim([-grid_size, grid_size])
    ax.set_ylim([-grid_size, grid_size])
    ax.set_zlim([-grid_size, grid_size])

    if(show):
        plt.show()

def plot_m0(roi, points = 10, ax = None, show=True):
    if(show):
        fig = plt.figure(figsize=(8, 6))
        ax = fig.add_subplot(111, projection='3d')

    grid_size = 0.005
    x = np.linspace(-grid_size/1, grid_size/1, int(points/1))
    y = np.linspace(-grid_size/1, grid_size/1, int(points/1))
    z = np.linspace(-grid_size, 0, int(points/2))
    X, Y, Z = np.meshgrid(x, y, z)

    observation_points = np.vstack((X.ravel(), Y.ravel(), Z.ravel())).T

    Bx,By,Bz = roi.magnetization(observation_points, 1)
    roi.plot(color='b', linewidth=1.5, ax=ax,show=False)
    plot_magnetic_field(observation_points, Bx, By, Bz, ax=ax, show=False)
    ax.set_xlim([-grid_size, grid_size])
    ax.set_ylim([-grid_size, grid_size])
    ax.set_zlim([-grid_size, grid_size])

    if(show):
        plt.show()


# Example Usage
if __name__ == "__main__":
    roi_rad = 0.0034
    roi_top = 0.0015
    roi_bottom = 0.00075
    roi = ROI(roi_rad, roi_top, roi_bottom)
    # plot_m0(roi)

    ###############################################################'
    ##                Coil Comparative Simulations
    ###############################################################

    # coilsfig, ax = plt.subplots(1,3, subplot_kw=dict(projection='3d'))
    # print("Box: ", end="")
    # coil = Coil.boxspiral(0.0016, 0.008, 0.01, 20, 10000, center=(0,0,-0.0008),axis='z')
    # print(roi.integrate(coil)/coil.get_length())
    # plotcoilsim(coil,roi,ax=ax[0], show=False)

    # print("D: ", end="")
    # points_per_turn = 1000
    # coil = Coil.d(radius=0.005, points_per_turn=points_per_turn, center=(0,0,0))
    # print(roi.integrate(coil)/coil.get_length())
    # plotcoilsim(coil,roi,ax=ax[1], show=False)

    # print("Spiral: ", end="")
    # coil = Coil.spiral(radius=0.0015, pitch=0.0002, turns=8, center=(0.002,0,0))
    # print(roi.integrate(coil)/coil.get_length())
    # plotcoilsim(coil,roi,ax=ax[2], show=False)

    ###############################################################'
    ##                Surface Flux Optimization
    ###############################################################

    #define the coil plane
    grid_size = 0.01
    points = 50
    x = np.linspace(-grid_size, grid_size, points)
    y = np.linspace(-grid_size, grid_size, points)
    z = np.linspace(0, 0, 1)
    X, Y, Z = np.meshgrid(x, y, z)
    Bx, By, Bz = np.zeros_like(X), np.zeros_like(Y), np.zeros_like(Z)
    observation_points = np.vstack((X.ravel(), Y.ravel(), Z.ravel())).T
    Bx,By,Bz = roi.magnetization(observation_points, 1)
    zmag = Bz.reshape(len(x),-1)

    #Plot Z flux over the plane
    fig, ax = plt.subplots()
    pcm = ax.pcolormesh(x, y, zmag)
    fig.colorbar(pcm, ax=ax)
    ellipse = Ellipse(xy=(0, 0), width=0.00865, height=0.00687, 
                        edgecolor='r', fc='None', lw=2)
    ax.add_patch(ellipse)
    plt.vlines(x=0,ymin=-0.00687/2,ymax=0.00687/2,color='r', linewidth=2)
    plt.show()

    def inside(x,y,length,width):
        return np.sqrt(x**2/(length/2)**2 + y**2/(width/2)**2) < 1
    
    def ellipse_perimeter(a, b): #(sigh)
        # Ramanujan's second approximation
        h = (3 * (a + b)) - np.sqrt((3 * a + b) * (a + 3 * b))
        return np.pi * h

    #Optimize seperately for X and Y coil sizes (length and width)
    lengths = np.linspace(grid_size/(points/2),2*grid_size, int(points/2))
    widths = np.linspace(grid_size/(points/2),2*grid_size, int(points/2))
    phi = np.zeros((len(lengths),len(widths)))
    for i in range(len(lengths)):
        for j in range(len(widths)):
            for x_idx in range(len(x)):
                for y_idx in range(len(y)):
                    if(inside(x[x_idx],y[y_idx], lengths[i], widths[j])):
                        if(y[y_idx] < 0):
                            phi[i,j] += zmag[x_idx, y_idx]
                        elif(y[y_idx] > 0):
                            phi[i,j] -= zmag[x_idx, y_idx]   
                        else:
                            print("zero!")
            #weight by coil length
            phi[i,j] = phi[i,j]/(ellipse_perimeter(lengths[i], widths[j]) + lengths[i]*2)

        #Plot Z flux over the plane
    fig, ax = plt.subplots()
    pcm = ax.pcolormesh(lengths, widths, phi)
    plt.xlabel("Length (m)")
    plt.ylabel("Width (m)")
    fig.colorbar(pcm, ax=ax)
    plt.show()